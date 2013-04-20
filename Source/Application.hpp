#ifndef APPLICATION_HPP_INCLUDED
#define APPLICATION_HPP_INCLUDED

#include <wx/stdpaths.h>
#include <wx/filename.h>

#include <fstream>

#include "ApplicationController.hpp"
#include "Configuration.hpp"
#include "Database.hpp"
#include "IniFile.hpp"
#include "Player.hpp"

class ResourceStringsLoader
{
public:
	static ResourceStrings LoadResourceStrings(const IniFile& iniFile)
	{
		std::string sectionName = "ResourceStrings";

		ResourceStringIds::Container resourceStringIds = ResourceStringIds::GetAll();
		ResourceStrings resourceStrings;

		for(ResourceStringIds::Iterator current = resourceStringIds.begin();
		    current != resourceStringIds.end();
		    ++current)
		{
			std::string value = iniFile.ReadString(sectionName, *current);
			resourceStrings.AddString(*current, value);
		}

		return resourceStrings;
	}
};

class ConfigurationLoader
{
public:
	static Configuration LoadConfiguration(const IniFile& iniFile)
	{
		std::string generalSectionName = "General";
		std::string operatingSystemSectionName = GetOperatingSystemSectionName();

		std::string savePath = iniFile.ReadString(generalSectionName, "SavePath");
		std::string playerPath = iniFile.ReadString(operatingSystemSectionName, "PlayerPath");

		Configuration configuration;
		configuration.SetPlayerPath(playerPath);
		configuration.SetSavePath(savePath);
		return configuration;
	}
private:
	static std::string GetOperatingSystemSectionName()
	{
#ifdef __WINDOWS__
		return "Windows";
#elif __LINUX__
		return "Linux";
#endif
	}
};

class Application : public wxApp
{
public:
	Application()
		: configuration_(LoadConfiguration())
		, database_(GetDatabaseFilePath())
		, player_(configuration_.GetPlayerPath())
		, streamrecorder_(player_, database_)
		, applicationController_(streamrecorder_)
	{
		streamrecorder_.SetSavePath(configuration_.GetSavePath());

		applicationController_.SetResourceStrings(LoadResourceStrings());

		SetEventHandlers();
	}

	virtual int OnRun()
	{
		applicationController_.Run();

		return wxApp::OnRun();
	}

	void OnConnecting(wxCommandEvent& event)
	{
		StationEvent& stationEvent = static_cast<StationEvent&>(event);
		int stationId = stationEvent.GetStationId();

		applicationController_.OnConnecting(stationId);
	}

	void OnRecordingStarted(wxCommandEvent& event)
	{
		StationEvent& stationEvent = static_cast<StationEvent&>(event);
		int stationId = stationEvent.GetStationId();

		applicationController_.OnRecordingStarted(stationId);
	}

	void OnRecordingFinished(wxCommandEvent& event)
	{
		StationEvent& stationEvent = static_cast<StationEvent&>(event);
		int stationId = stationEvent.GetStationId();

		applicationController_.OnRecordingFinished(stationId);

		if(applicationController_.IsShutdown())
		{
			ExitMainLoop();
		}
	}

	void OnRecordingError(wxCommandEvent& event)
	{
		RecordingErrorEvent& recordingErrorEvent = static_cast<RecordingErrorEvent&>(event);
		int stationId = recordingErrorEvent.GetStationId();
		applicationController_.OnRecordingError(stationId, recordingErrorEvent.GetRecordingError());
	}

	void OnTitleUpdate(wxCommandEvent& event)
	{
		TitleUpdateEvent& titleUpdateEvent = static_cast<TitleUpdateEvent&>(event);

		int stationId = titleUpdateEvent.GetStationId();
		const std::string& title = titleUpdateEvent.GetTitle();

		applicationController_.OnTitleUpdate(stationId, title);
	}
private:
	void SetEventHandlers()
	{
		Connect(wxID_ANY, EVT_CONNECTING, wxCommandEventHandler(Application::OnConnecting));
		Connect(wxID_ANY, EVT_RECORDING_STARTED, wxCommandEventHandler(Application::OnRecordingStarted));
		Connect(wxID_ANY, EVT_RECORDING_FINISHED, wxCommandEventHandler(Application::OnRecordingFinished));
		Connect(wxID_ANY, EVT_RECORDING_ERROR, wxCommandEventHandler(Application::OnRecordingError));
		Connect(wxID_ANY, EVT_TITLE_UPDATE, wxCommandEventHandler(Application::OnTitleUpdate));
	}

	static ResourceStrings LoadResourceStrings()
	{
		const std::string filePath = GetDataFilePath("resourcestrings.ini");

		std::ifstream iniFileStream(filePath.c_str());
		IniFile iniFile(iniFileStream);
		return ResourceStringsLoader::LoadResourceStrings(iniFile);
	}

	static Configuration LoadConfiguration()
	{
		const std::string configFilePath = GetDataFilePath("config.ini");
		std::ifstream iniFileStream(configFilePath.c_str());
		IniFile iniFile(iniFileStream);
		return ConfigurationLoader::LoadConfiguration(iniFile);
	}

	static std::string GetDatabaseFilePath()
	{
		return GetDataFilePath("stations.db");
	}

	static std::string GetDataFilePath(const std::string& filename)
	{
		wxString executablePath = wxStandardPaths::Get().GetExecutablePath();

		wxFileName executablePathFileName(executablePath);

		wxFileName result(executablePathFileName.GetPath(), filename);

		return std::string(result.GetFullPath());
	}
private:
	Configuration configuration_;
	Database database_;
	Player player_;
	Streamrecorder streamrecorder_;
	ApplicationController applicationController_;
};

#endif