#ifndef MAINCONTROLLER_HPP_INCLUDED
#define MAINCONTROLLER_HPP_INCLUDED

#include <vector>

#include "Streamrecorder.hpp"

template<class TMainView>
class MainController
{
public:
	MainController(Streamrecorder& streamrecorder)
		: streamrecorder_(streamrecorder)
		, pView_(NULL)
	{
	}

	void SetView(TMainView* pView)
	{
		pView_ = pView;
	}

	void AddStations(const std::vector<Station>& stations)
	{
		pView_->AddStations(stations);

		typedef std::vector<Station>::const_iterator StationIterator;

		for(StationIterator current = stations.begin(); current != stations.end(); ++current)
		{
			const Station& station = *current;

			streamrecorder_.AddStationToPlaylist(station);
		}
	}

	void OnStationSelected(const Station& station)
	{
		if(streamrecorder_.IsRecording(station.GetId()))
		{
			pView_->DisableRecording();
		}
		else
		{
			pView_->EnableRecording();
		}

		pView_->EnablePlaying();
		pView_->EnableDeleting();
	}

	void OnStationDeselected()
	{
		pView_->DisableStationActions();
	}

	void OnPlayStation(const Station& station) const
	{
		try
		{
			streamrecorder_.PlayStation(station);
		}
		catch(const PlayerError& playerError)
		{
			pView_->ShowPlayerError(playerError);
		}
	}

	void OnRecordStation(const Station& station)
	{
		pView_->DisableRecording();

		EventHandler eventHandler(station.GetId());
		streamrecorder_.StartRecording(station, eventHandler);
	}

	void OnStopRecordStation(const Station& station)
	{
		streamrecorder_.StopRecording(station.GetId());

		pView_->ClearStationStatus(station.GetId());
		pView_->ClearStationTitle(station.GetId());
		pView_->DisableStopRecording();
	}

	void OnRecordingFinished(int stationId)
	{
		if(streamrecorder_.GetRecorderStatus(stationId) == RecorderStatus::StreamEnd)
		{
			pView_->SetStreamEndedStatus(stationId);
		}

		pView_->ClearStationTitle(stationId);

		streamrecorder_.DeleteRecorder(stationId);

		if(pView_->IsStationSelected(stationId))
		{
			pView_->EnableRecording();
		}
	}

	void OnDeleteStation(const Station& station)
	{
		streamrecorder_.RemoveStationFromPlaylist(station.GetId());
		pView_->RemoveSelectedStation();
		pView_->DisableStationActions();
	}

	void OnClose(bool& canClose)
	{
		streamrecorder_.ShutdownAsync();

		if(!streamrecorder_.IsShutdown())
		{
			canClose = false;
			pView_->Hide();
		}
	}

	void OnQuit()
	{
		pView_->Close();
	}

	void ShowPlaylist()
	{
		std::vector<Station> stations = streamrecorder_.GetStationsOnPlaylist();
		pView_->AddStations(stations);
		pView_->DisableStationActions();
	}

	void OnConnecting(int stationId)
	{
		pView_->SetConnectingStatus(stationId);
	}

	void OnRecordingStarted(int stationId)
	{
		pView_->SetConnectedStatus(stationId);
	}

	void OnTitleUpdate(int stationId, const std::string& title)
	{
		pView_->SetStationTitle(stationId, title);
	}

	void OnRecordingError(int stationId, const RecordingError& recordingError)
	{
		pView_->SetStationError(stationId, recordingError);
		pView_->EnableRecording();
	}

	void OnAddStation(Station& station)
	{
		streamrecorder_.AddStation(station);
		streamrecorder_.AddStationToPlaylist(station);
		pView_->AddStation(station);
	}

	void OnOpenRecordingFolder()
	{
		streamrecorder_.OpenRecordingFolder();
	}

	void OnOpenRecordingFolder(const Station& station)
	{
		streamrecorder_.OpenRecordingFolder(station);
	}

	void OnRecordAllStation(const std::vector<Station>& stations)
	{
		pView_->DisableRecording();

		typedef std::vector<Station>::const_iterator StationIterator;

		for(StationIterator current = stations.begin(); current != stations.end(); ++current)
		{
			const Station& station = *current;

			EventHandler eventHandler(station.GetId());
			streamrecorder_.StartRecording(station, eventHandler);
		}
	}
private:
	Streamrecorder& streamrecorder_;
	TMainView* pView_;
};

#endif