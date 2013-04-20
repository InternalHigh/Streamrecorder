#ifndef APPLICATIONCONTROLLER_HPP_INCLUDED
#define APPLICATIONCONTROLLER_HPP_INCLUDED

#include <vector>

#include "ResourceStrings.hpp"
#include "StationDatabaseView.hpp"
#include "StationDatabaseController.hpp"
#include "Streamrecorder.hpp"
#include "MainController.hpp"
#include "MainView.hpp"

class ApplicationController
{
public:
	ApplicationController(Streamrecorder& streamrecorder)
		: streamrecorder_(streamrecorder)
		, mainController_(streamrecorder_)
		, stationDatabaseController_(streamrecorder)
	{
	}

	void Run()
	{
		MainView<ApplicationController>* pMainView = new MainView<ApplicationController>(resourceStrings_);
		pMainView->SetEventHandler(this);

		mainController_.SetView(pMainView);
		mainController_.ShowPlaylist();
	}

	void SetResourceStrings(const ResourceStrings& resourceStrings)
	{
		resourceStrings_ = resourceStrings;
	}

	void OnQuit()
	{
		mainController_.OnQuit();
	}

	void OnStationSelected(const Station& station)
	{
		mainController_.OnStationSelected(station);
	}

	void OnStationDeselected()
	{
		mainController_.OnStationDeselected();
	}

	void OnDeleteStation(const Station& station)
	{
		mainController_.OnDeleteStation(station);
	}

	void OnPlayStation(const Station& station) const
	{
		mainController_.OnPlayStation(station);
	}

	void OnStopRecordStation(const Station& station)
	{
		mainController_.OnStopRecordStation(station);
	}

	void OnRecordStation(const Station& station)
	{
		mainController_.OnRecordStation(station);
	}

	void OnClose(bool& canClose)
	{
		mainController_.OnClose(canClose);
	}

	void OnConnecting(int stationId)
	{
		mainController_.OnConnecting(stationId);
	}

	void OnRecordingStarted(int stationId)
	{
		mainController_.OnRecordingStarted(stationId);
	}

	void OnRecordingFinished(int stationId)
	{
		mainController_.OnRecordingFinished(stationId);
	}

	void OnRecordingError(int stationId, const RecordingError& recordingError)
	{
		mainController_.OnRecordingError(stationId, recordingError);
	}

	void OnTitleUpdate(int stationId, const std::string& title)
	{
		mainController_.OnTitleUpdate(stationId, title);
	}

	void OnValidateStation(const Station& station, bool& isStationValid)
	{
		stationDatabaseController_.OnValidateStation(station, isStationValid);
	}

	void OnAddStation(Station& station)
	{
		mainController_.OnAddStation(station);
	}

	void OnOpenRecordingFolder()
	{
		mainController_.OnOpenRecordingFolder();
	}

	void OnOpenRecordingFolder(const Station& station)
	{
		mainController_.OnOpenRecordingFolder(station);
	}

	bool IsShutdown() const
	{
		return streamrecorder_.IsShutdown();
	}

	void OnOpenStationDatabaseDialog()
	{
		StationDatabaseView<ApplicationController> stationDatabaseView(resourceStrings_);
		stationDatabaseView.SetEventHandler(this);

		stationDatabaseController_.SetView(&stationDatabaseView);
		stationDatabaseController_.LoadStations();
		std::vector<Station> selectedStations = stationDatabaseController_.WaitForStationSelection();
		mainController_.AddStations(selectedStations);
	}
private:
	Streamrecorder& streamrecorder_;
	ResourceStrings resourceStrings_;
	MainController< MainView<ApplicationController> > mainController_;
	StationDatabaseController< StationDatabaseView<ApplicationController> > stationDatabaseController_;
};

#endif