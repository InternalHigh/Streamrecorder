#ifndef RECORDER_HPP_INCLUDED
#define RECORDER_HPP_INCLUDED

#include <wx/thread.h>
#include <curl/curl.h>

#include "RecordingError.hpp"
#include "ShoutcastStreamHandler.hpp"
#include "ShoutcastStream.hpp"

class RecorderStatus
{
public:
	enum Value
	{
		NotStarted,
		StreamEnd,
		Error
	};
};

class Recorder : private wxThread
{
public:
	Recorder(const EventHandler& eventHandler)
		: wxThread(wxTHREAD_JOINABLE)
		, stop_(false)
		, connected_(false)
		, requiredHeadersReceived_(false)
		, recorderStatus_(RecorderStatus::NotStarted)
		, eventHandler_(eventHandler)
		, shoutcastStreamHandler_(streamWriter_, eventHandler_)
		, shoutcastStream_(shoutcastStreamHandler_)
		, pCurl_(NULL)
	{
	}

	~Recorder()
	{
		assert(!IsAlive());
	}

	void Start(const std::string& streamUrl, const std::string& filePath, const std::string& fileName)
	{
		assert(!IsAlive());

		streamUrl_ = streamUrl;
		filePath_ = filePath;
		fileName_ = fileName;

		Create();
		Run();
	}

	void StopAsync()
	{
		stop_ = true;
	}

	void WaitForStopped()
	{
		Wait();
	}

	RecorderStatus::Value GetStatus()
	{
		return recorderStatus_;
	}
private:
	ExitCode Entry()
	{
		pCurl_ = curl_easy_init();
		
		curl_easy_setopt(pCurl_, CURLOPT_URL, streamUrl_.c_str());
		
		curl_easy_setopt(pCurl_, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(pCurl_, CURLOPT_WRITEDATA, this);

		curl_easy_setopt(pCurl_, CURLOPT_PROGRESSFUNCTION, ProgressCallback);
		curl_easy_setopt(pCurl_, CURLOPT_PROGRESSDATA, this);
		curl_easy_setopt(pCurl_, CURLOPT_NOPROGRESS, 0);

		curl_easy_setopt(pCurl_, CURLOPT_HEADERFUNCTION, HeaderCallback);
		curl_easy_setopt(pCurl_, CURLOPT_HEADERDATA, this);

		curl_easy_setopt(pCurl_, CURLOPT_LOW_SPEED_TIME, 30);
		curl_easy_setopt(pCurl_, CURLOPT_LOW_SPEED_LIMIT, 1);

		curl_slist* pHttp200Aliases = NULL;
		pHttp200Aliases = curl_slist_append(pHttp200Aliases, "ICY 200 OK");
		curl_easy_setopt(pCurl_, CURLOPT_HTTP200ALIASES, pHttp200Aliases);

		curl_slist* pHttpHeaders = NULL;
		pHttpHeaders = curl_slist_append(pHttpHeaders, "Icy-MetaData: 1");
		curl_easy_setopt(pCurl_, CURLOPT_HTTPHEADER, pHttpHeaders);

		curl_easy_setopt(pCurl_, CURLOPT_FOLLOWLOCATION, 1);

		eventHandler_.OnConnecting();

		CURLcode result = curl_easy_perform(pCurl_);

		if(result != 0 && result != CURLE_ABORTED_BY_CALLBACK)
		{
			std::string errorMessage = curl_easy_strerror(result);
			RecordingError recordingError(RecordingErrorCode::ConnectFailed, errorMessage);
			SetError(recordingError);
		}
		
		if(recorderStatus_ != RecorderStatus::Error)
		{
			recorderStatus_ = RecorderStatus::StreamEnd;
		}

		curl_slist_free_all(pHttp200Aliases);
		curl_slist_free_all(pHttpHeaders);

		curl_easy_cleanup(pCurl_);

		eventHandler_.OnRecordingFinished();

		return 0;
	}
	
	void OnHeaderReceived(const char* pHeader, std::size_t bufferSize)
	{
		std::string header(pHeader, bufferSize);

		std::size_t metadataInterval;

		if(!MetadataParser::ParseMetadataIntervalHeader(header, metadataInterval))
			return;
		
		shoutcastStream_.SetMetadataInterval(metadataInterval);

		char* pContentType;
		if(curl_easy_getinfo(pCurl_, CURLINFO_CONTENT_TYPE, &pContentType) != CURLE_OK)
			return;

		requiredHeadersReceived_ = true;

		wxFileName::Mkdir(filePath_, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		
		wxDateTime now = wxDateTime::Now();
		wxString timestamp = now.Format("%Y-%m-%d %H-%M-%S");
		std::string fileExtension = GetFileExtensionByContentType(pContentType);

		wxFileName pathBuilder = wxFileName(filePath_, fileName_ + " " + timestamp, fileExtension);
		std::string fullPath = std::string(pathBuilder.GetFullPath());

		try
		{
			streamWriter_.OpenFile(fullPath);
		}
		catch(const RecordingError& recordingError)
		{
			SetError(recordingError);
		}
	}

	std::string GetFileExtensionByContentType(const char* pContentType)
	{
		const std::string unknown = "unknown";

		if(!pContentType)
			return unknown;

		std::string contentType(pContentType);

		if(contentType.find("mpeg") != std::string::npos)
			return "mp3";

		if(contentType.find("aac") != std::string::npos)
			return "aac";

		return unknown;
	}

	void OnDataReceived(const char* pBuffer, std::size_t bufferSize)
	{
		if(recorderStatus_ == RecorderStatus::Error)
			return;

		if(!connected_)
		{
			eventHandler_.OnRecordingStarted();
			connected_ = true;
		}

		if(!requiredHeadersReceived_)
		{
			RecordingError recordingError(RecordingErrorCode::InvalidStream, "");
			SetError(recordingError);
			return;
		}

		try
		{
			shoutcastStream_.ProcessBuffer(pBuffer, bufferSize);
		}
		catch(const RecordingError& recordingError)
		{
			SetError(recordingError);
		}
	}

	static int ProgressCallback(Recorder* pRecorder, double, double, double, double)
	{
		return pRecorder->stop_;
	}

	static size_t HeaderCallback(const char* pHeader, size_t, size_t bufferSize, Recorder* pRecorder)
	{
		pRecorder->OnHeaderReceived(pHeader, bufferSize);
		return bufferSize;
	}

	static size_t WriteCallback(const char* pBuffer, size_t, size_t bufferSize, Recorder* pRecorder)
	{
		pRecorder->OnDataReceived(pBuffer, bufferSize);
		return bufferSize;
	}
	
	void SetError(const RecordingError& recordingError)
	{
		recorderStatus_ = RecorderStatus::Error;
		eventHandler_.OnRecordingError(recordingError);
		StopAsync();
	}
private:
	bool stop_;
	bool connected_;
	bool requiredHeadersReceived_;
	RecorderStatus::Value recorderStatus_;

	std::string streamUrl_;
	std::string filePath_;
	std::string fileName_;

	EventHandler eventHandler_;
	StreamWriter streamWriter_;
	ShoutcastStreamHandler shoutcastStreamHandler_;
	ShoutcastStream<ShoutcastStreamHandler> shoutcastStream_;

	CURL* pCurl_;
};

#endif