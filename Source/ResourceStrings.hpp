#ifndef RESOURCESTRINGS_HPP_INCLUDED
#define RESOURCESTRINGS_HPP_INCLUDED

#include <string>
#include <map>
#include <vector>
#include <cassert>

class ResourceStringIds
{
public:
	typedef std::vector<const char*> Container;
	typedef Container::const_iterator Iterator;
	
	static const char* ApplicationTitle()
	{
		return "ApplicationTitle";
	}

	static const char* MenuFile()
	{
		return "MenuFile";
	}

	static const char* MenuQuit()
	{
		return "MenuQuit";
	}

	static const char* Station()
	{
		return "Station";
	}

	static const char* Status()
	{
		return "Status";
	}

	static const char* Title()
	{
		return "Title";
	}

	static const char* StationDatabase()
	{
		return "StationDatabase";
	}

	static const char* StartRecording()
	{
		return "StartRecording";
	}

	static const char* StopRecording()
	{
		return "StopRecording";
	}

	static const char* Play()
	{
		return "Play";
	}

	static const char* DeleteStation()
	{
		return "DeleteStation";
	}

	static const char* StatusConnecting()
	{
		return "StatusConnecting";
	}

	static const char* StatusRecording()
	{
		return "StatusRecording";
	}

	static const char* Error()
	{
		return "Error";
	}

	static const char* Url()
	{
		return "Url";
	}

	static const char* Format()
	{
		return "Format";
	}

	static const char* StreamUrl()
	{
		return "StreamUrl";
	}

	static const char* AddSelectedStationsToPlaylist()
	{
		return "AddSelectedStationsToPlaylist";
	}

	static const char* StreamEnded()
	{
		return "StreamEnded";
	}

	static const char* ConnectFailed()
	{
		return "ConnectFailed";
	}

	static const char* FileOpenFailed()
	{
		return "FileOpenFailed";
	}

	static const char* FileWriteFailed()
	{
		return "FileWriteFailed";
	}

	static const char* InvalidStream()
	{
		return "InvalidStream";
	}

	static const char* PlayerNotAvailable()
	{
		return "PlayerNotAvailable";
	}

	static const char* PlayerExecutionFailed()
	{
		return "PlayerExecutionFailed";
	}

	static const char* OpenRecordingFolder()
	{
		return "OpenRecordingFolder";
	}

	static const char* OpenRecordingFolderForStation()
	{
		return "OpenRecordingFolderForStation";
	}

	static const char* StationNameMustNotBeEmpty()
	{
		return "StationNameMustNotBeEmpty";
	}

	static const char* StreamUrlMustNotBeEmpty()
	{
		return "StreamUrlMustNotBeEmpty";
	}

	static const char* AddStation()
	{
		return "AddStation";
	}

	static const char* StationName()
	{
		return "StationName";
	}

	static Container GetAll()
	{
		std::vector<const char*> allIds;

		allIds.push_back(ApplicationTitle());
		allIds.push_back(MenuFile());
		allIds.push_back(MenuQuit());
		allIds.push_back(Station());
		allIds.push_back(Status());
		allIds.push_back(Title());
		allIds.push_back(StationDatabase());
		allIds.push_back(StartRecording());
		allIds.push_back(StopRecording());
		allIds.push_back(Play());
		allIds.push_back(DeleteStation());
		allIds.push_back(StatusConnecting());
		allIds.push_back(StatusRecording());
		allIds.push_back(Error());
		allIds.push_back(Url());
		allIds.push_back(Format());
		allIds.push_back(StreamUrl());
		allIds.push_back(AddSelectedStationsToPlaylist());
		allIds.push_back(StreamEnded());
		allIds.push_back(ConnectFailed());
		allIds.push_back(FileOpenFailed());
		allIds.push_back(FileWriteFailed());
		allIds.push_back(InvalidStream());
		allIds.push_back(PlayerNotAvailable());
		allIds.push_back(PlayerExecutionFailed());
		allIds.push_back(OpenRecordingFolder());
		allIds.push_back(OpenRecordingFolderForStation());
		allIds.push_back(StationNameMustNotBeEmpty());
		allIds.push_back(StreamUrlMustNotBeEmpty());
		allIds.push_back(AddStation());
		allIds.push_back(StationName());

		return allIds;
	}
};

class ResourceStrings
{
public:
	void AddString(const std::string& resourceStringId, const std::string& value)
	{
		resourceStringMap_[resourceStringId] = value;
	}

	std::string operator[](const std::string& resourceStringId) const
	{
		Map::const_iterator iter = resourceStringMap_.find(resourceStringId);
		
		if(iter == resourceStringMap_.end())
			return "-";

		return iter->second;
	}
private:
	typedef std::map<std::string, std::string> Map;
	Map resourceStringMap_;
};

#endif