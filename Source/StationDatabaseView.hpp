#ifndef STATIONDATABASEVIEW_HPP_INCLUDED
#define STATIONDATABASEVIEW_HPP_INCLUDED

#include <wx/wx.h>
#include <wx/listctrl.h>

#include "ResourceStrings.hpp"
#include "Station.hpp"

template<class TEventHandler>
class StationDatabaseView : public wxDialog
{
public:
	StationDatabaseView(const ResourceStrings& resourceStrings)
		: wxDialog(NULL, wxID_ANY, resourceStrings[ResourceStringIds::StationDatabase()], wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX | wxMINIMIZE_BOX)
		, resourceStrings_(resourceStrings)
	{
		CreateControls();
		SetEventHandlers();
	}

	void SetEventHandler(TEventHandler* pEventHandler)
	{
		pEventHandler_ = pEventHandler;
	}

	void SetStations(const std::vector<Station>& stations)
	{
		stations_ = stations;

		typedef std::vector<Station>::const_iterator StationIterator;
		
		StationIterator begin = stations_.begin();
		StationIterator end = stations_.end();

		for(StationIterator current = begin; current != end; ++current)
		{
			const Station& station = *current;

			int index = current - begin;

			pListCtrl_->InsertItem(index, station.GetName());
			pListCtrl_->SetItem(index, 1, station.GetHomepageUrl());
			pListCtrl_->SetItem(index, 2, station.GetStreamUrl());
		}

		pListCtrl_->SetColumnWidth(0, 120);
		pListCtrl_->SetColumnWidth(1, 200);
		pListCtrl_->SetColumnWidth(2, 300);

		CreateSizers();
	}

	std::vector<Station> WaitForStationSelection()
	{
		ShowModal();
		return selectedStations_;
	}

	void ShowStationNameIsEmptyError()
	{
		wxMessageBox(resourceStrings_[ResourceStringIds::StationNameMustNotBeEmpty()], wxMessageBoxCaptionStr, wxOK | wxICON_ERROR);
	}

	void ShowStreamUrlIsEmptyError()
	{
		wxMessageBox(resourceStrings_[ResourceStringIds::StreamUrlMustNotBeEmpty()], wxMessageBoxCaptionStr, wxOK | wxICON_ERROR);
	}
private:
	void CreateControls()
	{
		pListCtrl_ = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);
		pListCtrl_->InsertColumn(0, resourceStrings_[ResourceStringIds::Station()]);
		pListCtrl_->InsertColumn(1, resourceStrings_[ResourceStringIds::Url()]);
		pListCtrl_->InsertColumn(2, resourceStrings_[ResourceStringIds::StreamUrl()]);

		pButtonAddToPlaylist_ = new wxButton(this, wxID_OK, resourceStrings_[ResourceStringIds::AddSelectedStationsToPlaylist()]);

		pStaticTextStationName_ = new wxStaticText(this, wxID_ANY, resourceStrings_[ResourceStringIds::StationName()] + ":");
		pTextCtrlStationName_ = new wxTextCtrl(this, wxID_ANY);
		pStaticTextStationStreamUrl_ = new wxStaticText(this, wxID_ANY, resourceStrings_[ResourceStringIds::StreamUrl()] + ":");
		pTextCtrlStationStreamUrl_ = new wxTextCtrl(this, wxID_ANY);

		pButtonAddStation_ = new wxButton(this, wxID_ANY, resourceStrings_[ResourceStringIds::AddStation()]);
	}

	void CreateSizers()
	{
		wxFlexGridSizer* pGridSizer = new wxFlexGridSizer(2, 2, 5, 5);
		pGridSizer->Add(pStaticTextStationName_, 0, wxALIGN_CENTER_VERTICAL);
		pGridSizer->Add(pTextCtrlStationName_, 0, wxEXPAND);
		pGridSizer->Add(pStaticTextStationStreamUrl_, 0, wxALIGN_CENTER_VERTICAL);
		pGridSizer->Add(pTextCtrlStationStreamUrl_, 0, wxEXPAND);
		pGridSizer->AddGrowableCol(1);

		wxBoxSizer* pVBox = new wxBoxSizer(wxVERTICAL);
		pVBox->Add(pListCtrl_, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);
		pVBox->Add(pButtonAddToPlaylist_, 0, wxALIGN_CENTER | wxALL, 5);
		pVBox->AddSpacer(20);
		pVBox->Add(pGridSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 15);	
		pVBox->Add(pButtonAddStation_, 0, wxALIGN_CENTER | wxALL, 5);
		pVBox->SetSizeHints(this);

		SetSizer(pVBox);
	}

	void SetEventHandlers()
	{
		Connect(pListCtrl_->GetId(), wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxCommandEventHandler(StationDatabaseView::OnStationsSelected));
		Connect(pButtonAddToPlaylist_->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(StationDatabaseView::OnStationsSelected));
		Connect(pButtonAddStation_->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(StationDatabaseView::OnAddStation));
	}

	void OnStationsSelected(wxCommandEvent&)
	{
		SetSelectedStations();
		EndModal(0);
	}

	void OnAddStation(wxCommandEvent&)
	{
		std::string stationName(pTextCtrlStationName_->GetValue());
		std::string stationStreamUrl(pTextCtrlStationStreamUrl_->GetValue());

		Station station(0, stationName, "", stationStreamUrl);

		bool isStationValid = true;;

		pEventHandler_->OnValidateStation(station, isStationValid);

		if(!isStationValid)
			return;

		pEventHandler_->OnAddStation(station);

		EndModal(0);
	}

	void SetSelectedStations()
	{
		selectedStations_.clear();

		long itemIndex = -1;

		for(;;)
		{
			itemIndex = pListCtrl_->GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

			if(itemIndex == -1)
				break;

			std::size_t stationIndex = static_cast<std::size_t>(itemIndex);
			const Station& station = stations_[stationIndex];
			selectedStations_.push_back(station);
		}
	}
private:
	wxListCtrl* pListCtrl_;
	wxButton* pButtonAddToPlaylist_;

	wxStaticText* pStaticTextStationName_;
	wxTextCtrl* pTextCtrlStationName_;
	wxStaticText* pStaticTextStationStreamUrl_;
	wxTextCtrl* pTextCtrlStationStreamUrl_;
	wxButton* pButtonAddStation_;
	
	std::vector<Station> stations_;
	std::vector<Station> selectedStations_;

	const ResourceStrings& resourceStrings_;
	TEventHandler* pEventHandler_;
};

#endif