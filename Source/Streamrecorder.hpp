#ifndef STREAMRECORDER_HPP_INCLUDED
#define STREAMRECORDER_HPP_INCLUDED

#include <wx/filename.h>

#include "Database.hpp"
#include "Player.hpp"
#include "Recorder.hpp"
#include "StationIdToRecorderMap.hpp"

class Streamrecorder
{
public:
	Streamrecorder(const Player& player, const Database& database)
		: player_(player)
		, database_(database)
		, wasShutdownCalled_(false)
	{
	}

	void ShutdownAsync()
	{
		wasShutdownCalled_ = true;

		typedef StationIdToRecorderMap::Iterator RecorderIterator;

		RecorderIterator begin = recorderMap_.GetBeginIterator();
		RecorderIterator end = recorderMap_.GetEndIterator();

		for(RecorderIterator current = begin; current != end; ++current)
		{
			Recorder* pRecorder = current->second;
			pRecorder->StopAsync();
		}
	}

	bool IsShutdown() const
	{
		return wasShutdownCalled_ && recorderMap_.IsEmpty();
	}

	void SetSavePath(const std::string& savePath)
	{
		savePath_ = savePath;
	}

	void AddStationToPlaylist(const Station& station) const
	{
		database_.AddStationToPlaylist(station);
	}

	void AddStation(Station& station) const
	{
		return database_.AddStation(station);
	}

	std::vector<Station> GetStationsOnPlaylist() const
	{
		return database_.GetStationsOnPlaylist();
	}

	std::vector<Station> GetStationsNotOnPlaylist() const
	{
		return database_.GetStationsNotOnPlaylist();
	}

	void StartRecording(const Station& station, const EventHandler& eventHandler)
	{
		if(IsRecording(station.GetId()))
			return;

		Recorder* pRecorder = new Recorder(eventHandler);
		recorderMap_.Add(station.GetId(), pRecorder);

		std::string streamUrl = station.GetStreamUrl();
		std::string recordingFolderPath = GetRecordingFolderPath(station);
		std::string stationNamePathComponent = GetStationNamePathComponent(station);

		pRecorder->Start(streamUrl, recordingFolderPath, stationNamePathComponent);
	}

	void StopRecording(int stationId)
	{
		Recorder* pRecorder = recorderMap_.GetRecorder(stationId);

		if(!pRecorder)
			return;

		pRecorder->StopAsync();
	}

	void RemoveStationFromPlaylist(int stationId)
	{
		StopRecording(stationId);

		database_.RemoveStationFromPlaylist(stationId);
	}

	void DeleteRecorder(int stationId)
	{
		Recorder* pRecorder = recorderMap_.GetRecorder(stationId);

		if(!pRecorder)
			return;

		pRecorder->WaitForStopped();

		delete pRecorder;
		
		recorderMap_.Remove(stationId);
	}

	RecorderStatus::Value GetRecorderStatus(int stationId)
	{
		return recorderMap_.GetRecorder(stationId)->GetStatus();
	}

	bool IsRecording(int stationId)
	{
		return recorderMap_.GetRecorder(stationId) != NULL;
	}

	void PlayStation(const Station& station) const
	{
		player_.Play(station);
	}

	void OpenRecordingFolder()
	{
		OpenFolder(savePath_);
	}

	void OpenRecordingFolder(const Station& station)
	{
		OpenFolder(GetRecordingFolderPath(station));
	}
private:
	static void OpenFolder(const std::string& path)
	{
		wxFileName pathBuilder(path);
		pathBuilder.Normalize();
		wxString folderPath(pathBuilder.GetFullPath());

		wxFileName::Mkdir(folderPath, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
		wxLaunchDefaultBrowser(folderPath);
	}

	static std::string GetStationNamePathComponent(const Station& station)
	{
		return ConvertStringToPathComponent(station.GetName(), 50);
	}

	std::string GetRecordingFolderPath(const Station& station)
	{
		std::string stationNamePathComponent = GetStationNamePathComponent(station);
		wxFileName pathBuilder = wxFileName(savePath_, stationNamePathComponent);
		return std::string(pathBuilder.GetFullPath());
	}

	static std::string ConvertStringToPathComponent(const std::string& inputString, size_t maxLength)
	{
		wxString outputString(inputString);

		wxString forbiddenChars = wxFileName::GetForbiddenChars();

		for(size_t i = 0; i < forbiddenChars.length(); ++i)
		{
			outputString.Replace(forbiddenChars.GetChar(i), " ");
		}

		outputString.Truncate(maxLength);
		outputString.Trim();

		return std::string(outputString);
	}
private:
	const Player& player_;
	const Database& database_;
	std::string savePath_;
	StationIdToRecorderMap recorderMap_;
	bool wasShutdownCalled_;
};

#endif