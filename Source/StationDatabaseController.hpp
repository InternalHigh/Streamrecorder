#ifndef STATIONDATABASECONTROLLER_HPP_INCLUDED
#define STATIONDATABASECONTROLLER_HPP_INCLUDED

#include "Streamrecorder.hpp"

template<class TStationDatabaseView>
class StationDatabaseController
{
public:
	StationDatabaseController(const Streamrecorder& streamrecorder)
		: streamrecorder_(streamrecorder)
		, pView_(NULL)
	{
	}

	void SetView(TStationDatabaseView* pView)
	{
		pView_ = pView;
	}

	void LoadStations()
	{
		std::vector<Station> stations = streamrecorder_.GetStationsNotOnPlaylist();
		pView_->SetStations(stations);
	}

	std::vector<Station> WaitForStationSelection()
	{
		return pView_->WaitForStationSelection();
	}

	void OnValidateStation(const Station& station, bool& isStationValid)
	{
		if(station.GetName() == "")
		{
			isStationValid = false;
			pView_->ShowStationNameIsEmptyError();
		}
		else if(station.GetStreamUrl() == "")
		{
			isStationValid = false;
			pView_->ShowStreamUrlIsEmptyError();
		}
	}
private:
	const Streamrecorder& streamrecorder_;
	TStationDatabaseView* pView_;
};

#endif