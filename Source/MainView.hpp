#ifndef MAINVIEW_HPP_INCLUDED
#define MAINVIEW_HPP_INCLUDED

#include "ResourceStrings.hpp"
#include "Station.hpp"

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <vector>

template<class TEventHandler>
class MainView : public wxFrame
{
public:
	MainView(const ResourceStrings& resourceStrings)
		: wxFrame(NULL, wxID_ANY, resourceStrings[ResourceStringIds::ApplicationTitle()])
		, resourceStrings_(resourceStrings)
		, pEventHandler_(NULL)
	{
		CreateControls();
		CreateSizers();
		SetEventHandlers();
		Show();
	}

	void SetEventHandler(TEventHandler* pEventHandler)
	{
		pEventHandler_ = pEventHandler;
	}

	void AddStation(const Station& station)
	{
		int itemIndex = pListCtrl_->GetItemCount();
		pListCtrl_->InsertItem(itemIndex, station.GetName());
		pListCtrl_->SetItemData(itemIndex, station.GetId());

		stations_.push_back(station);
	}

	void AddStations(const std::vector<Station>& stations)
	{
		for(std::vector<Station>::const_iterator current = stations.begin(); current != stations.end(); ++current)
		{
			AddStation(*current);
		}
	}

	void Close()
	{
		wxFrame::Close();
	}

	void Hide()
	{
		wxFrame::Hide();
	}

	bool IsStationSelected(int stationId)
	{
		long itemIndex = GetSelectedStationIndex();

		if(itemIndex == -1)
			return false;

		std::size_t stationIndex = static_cast<std::size_t>(itemIndex);

		return stations_[stationIndex].GetId() == stationId;
	}

	void RemoveSelectedStation()
	{
		long itemIndex = GetSelectedStationIndex();
		pListCtrl_->DeleteItem(itemIndex);
		stations_.erase(stations_.begin() + itemIndex);
	}

	void SetConnectingStatus(int stationId)
	{
		SetStationStatus(stationId, resourceStrings_[ResourceStringIds::StatusConnecting()]);
	}

	void SetConnectedStatus(int stationId)
	{
		SetStationStatus(stationId, resourceStrings_[ResourceStringIds::StatusRecording()]);
	}

	void SetStreamEndedStatus(int stationId)
	{
		SetStationStatus(stationId, resourceStrings_[ResourceStringIds::StreamEnded()]);
	}

	void ClearStationStatus(int stationId)
	{
		SetStationStatus(stationId, "");
	}

	void SetStationError(int stationId, const RecordingError& recordingError)
	{
		std::string statusMessage = resourceStrings_[ResourceStringIds::Error()];

		switch(recordingError.GetErrorCode())
		{
		case RecordingErrorCode::ConnectFailed:
			statusMessage += resourceStrings_[ResourceStringIds::ConnectFailed()];
			break;
		case RecordingErrorCode::FileOpenFailed:
			statusMessage += resourceStrings_[ResourceStringIds::FileOpenFailed()];
			break;
		case RecordingErrorCode::FileWriteFailed:
			statusMessage += resourceStrings_[ResourceStringIds::FileWriteFailed()];
			break;
		case RecordingErrorCode::InvalidStream:
			statusMessage += resourceStrings_[ResourceStringIds::InvalidStream()];
			break;
		}

		const std::string& errorMessage = recordingError.GetErrorMessage();

		if(!errorMessage.empty())
		{
			statusMessage += " (";
			statusMessage += errorMessage;
			statusMessage += ")";
		}

		SetStationStatus(stationId, statusMessage);
	}

	void SetStationTitle(int stationId, const std::string& title)
	{
		long itemIndex = GetItemIndexByStationId(stationId);

		if(itemIndex == -1)
			return;

		const int titleIndex = 2;

		pListCtrl_->SetItem(itemIndex, titleIndex, title);
	}

	void ClearStationTitle(int stationId)
	{
		SetStationTitle(stationId, "");
	}

	void EnableRecording()
	{
		pButtonRecord_->Enable();
		pButtonRecordStop_->Disable();
	}

	void DisableRecording()
	{
		pButtonRecord_->Disable();
		pButtonRecordStop_->Enable();
	}

	void DisableStopRecording()
	{
		pButtonRecordStop_->Disable();
	}

	void EnablePlaying()
	{
		pButtonPlay_->Enable();
	}

	void EnableDeleting()
	{
		pButtonDelete_->Enable();
	}

	void DisableStationActions()
	{
		pButtonRecord_->Disable();
		pButtonRecordStop_->Disable();
		pButtonPlay_->Disable();
		pButtonDelete_->Disable();
	}

	void ShowPlayerError(const PlayerError& playerError)
	{
		std::string errorMessage;

		switch(playerError.GetErrorCode())
		{
		case PlayerErrorCode::PlayerNotAvailable:
			errorMessage += resourceStrings_[ResourceStringIds::PlayerNotAvailable()];
			break;
		case PlayerErrorCode::PlayerExecutionFailed:
			errorMessage += resourceStrings_[ResourceStringIds::PlayerExecutionFailed()];
			break;
		}

		errorMessage += "\n\n" + playerError.GetMessage();

		wxMessageBox(errorMessage, wxMessageBoxCaptionStr, wxOK | wxICON_ERROR);
	}
private:
	void CreateControls()
	{
		wxMenuBar* pMenuBar = new wxMenuBar();
		wxMenu* pMenu = new wxMenu();
		pMenu->Append(ID_MENUITEM_QUIT, resourceStrings_[ResourceStringIds::MenuQuit()]);
		pMenuBar->Append(pMenu, resourceStrings_[ResourceStringIds::MenuFile()]);

		SetMenuBar(pMenuBar);

		pPanel_ = new wxPanel(this);

		pListCtrl_ = new wxListCtrl(pPanel_, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_SINGLE_SEL | wxLC_REPORT);
		pListCtrl_->InsertColumn(0, resourceStrings_[ResourceStringIds::Station()]);
		pListCtrl_->InsertColumn(1, resourceStrings_[ResourceStringIds::Status()], 0, 100);
		pListCtrl_->InsertColumn(2, resourceStrings_[ResourceStringIds::Title()]);
		pListCtrl_->SetColumnWidth(0, 120);
		pListCtrl_->SetColumnWidth(1, 120);
		pListCtrl_->SetColumnWidth(2, 550);

		pButtonStationDatabase_ = new wxButton(pPanel_, wxID_ANY, resourceStrings_[ResourceStringIds::StationDatabase()]);
		pButtonRecord_ = new wxButton(pPanel_, wxID_ANY, resourceStrings_[ResourceStringIds::StartRecording()]);
		pButtonRecordStop_ = new wxButton(pPanel_, wxID_ANY, resourceStrings_[ResourceStringIds::StopRecording()]);
		pButtonPlay_ = new wxButton(pPanel_, wxID_ANY, resourceStrings_[ResourceStringIds::Play()]);
		pButtonDelete_ = new wxButton(pPanel_, wxID_ANY, resourceStrings_[ResourceStringIds::DeleteStation()]);
	}

	void SetEventHandlers()
	{
		Connect(GetId(), wxEVT_CLOSE_WINDOW, wxCloseEventHandler(MainView::OnCloseWindow));

		Connect(pButtonStationDatabase_->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainView::btnStationdatabase_OnClick));

		Connect(pButtonRecord_->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainView::btnRecord_OnClick));
		Connect(pButtonRecordStop_->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainView::btnRecordStop_OnClick));
		Connect(pButtonPlay_->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainView::btnPlay_OnClick));
		Connect(pButtonDelete_->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MainView::btnDelete_OnClick));

		Connect(pListCtrl_->GetId(), wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(MainView::listctrl_OnItemSelected));
		Connect(pListCtrl_->GetId(), wxEVT_COMMAND_LIST_ITEM_DESELECTED, wxListEventHandler(MainView::listctrl_OnItemDeselected));
		Connect(pListCtrl_->GetId(), wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(MainView::listctrl_OnItemActivated));

		Connect(wxID_ANY, wxEVT_CONTEXT_MENU, wxContextMenuEventHandler(MainView::OnContextMenu));

		Connect(ID_MENUITEM_QUIT, wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler(MainView::menu_OnQuit));

		Connect(ID_MENUITEM_OPENRECORDINGFOLDER, wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler(MainView::menu_OnOpenRecordingFolder));
		Connect(ID_MENUITEM_OPENRECORDINGFOLDERFORSTATION, wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler(MainView::menu_OnOpenRecordingFolderForStation));
		Connect(ID_MENUITEM_RECORDALLSTATIONS, wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler(MainView::menu_OnRecordAllStations));
	}

	void CreateSizers()
	{
		wxBoxSizer* pVBox = new wxBoxSizer(wxVERTICAL);
		pVBox->Add(pListCtrl_, 1, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 15);

		wxBoxSizer* pHBox = new wxBoxSizer(wxHORIZONTAL);
		pHBox->Add(pButtonStationDatabase_, 0, wxALL, 5);
		pHBox->Add(pButtonRecord_, 0, wxALL, 5);
		pHBox->Add(pButtonRecordStop_, 0, wxALL, 5);
		pHBox->Add(pButtonPlay_, 0, wxALL, 5);
		pHBox->Add(pButtonDelete_, 0, wxALL, 5);

		pVBox->Add(pHBox, 0, wxALIGN_CENTER);
		pPanel_->SetSizer(pVBox);

		pVBox->SetSizeHints(this);
	}

	void OnCloseWindow(wxCloseEvent& event)
	{
		bool canClose = true;
		pEventHandler_->OnClose(canClose);

		if(canClose)
		{
			wxFrame::OnCloseWindow(event);
		}
		else
		{
			event.Veto();
		}
	}

	void SetStationStatus(int stationId, const std::string& statusString) const
	{
		long itemIndex = GetItemIndexByStationId(stationId);

		if(itemIndex == -1)
			return;

		pListCtrl_->SetItem(itemIndex, 1, statusString);
	}

	void btnStationdatabase_OnClick(wxCommandEvent&)
	{
		pEventHandler_->OnOpenStationDatabaseDialog();
	}

	void btnRecord_OnClick(wxCommandEvent&)
	{
		long itemIndex = GetSelectedStationIndex();
		Station station = GetStation(itemIndex);

		pEventHandler_->OnRecordStation(station);
	}

	void btnRecordStop_OnClick(wxCommandEvent&)
	{
		long itemIndex = GetSelectedStationIndex();
		Station station = GetStation(itemIndex);

		pEventHandler_->OnStopRecordStation(station);
	}

	void btnPlay_OnClick(wxCommandEvent&)
	{
		long itemIndex = GetSelectedStationIndex();
		Station station = GetStation(itemIndex);

		pEventHandler_->OnPlayStation(station);
	}

	void btnDelete_OnClick(wxCommandEvent&)
	{
		long itemIndex = GetSelectedStationIndex();

		if(itemIndex == -1)
			return;

		Station station = GetStation(itemIndex);

		pEventHandler_->OnDeleteStation(station);
	}

	void listctrl_OnItemSelected(wxListEvent& event)
	{
		long itemIndex = event.GetIndex();

		const Station& station = GetStation(itemIndex);

		pEventHandler_->OnStationSelected(station);
	}

	void listctrl_OnItemDeselected(wxListEvent&)
	{
		pEventHandler_->OnStationDeselected();
	}

	void listctrl_OnItemActivated(wxListEvent&)
	{
		long itemIndex = GetSelectedStationIndex();
		Station station = GetStation(itemIndex);

		pEventHandler_->OnRecordStation(station);
	}

	void OnContextMenu(wxContextMenuEvent& event)
	{
		wxMenu menu;
		menu.Append(ID_MENUITEM_OPENRECORDINGFOLDER, resourceStrings_[ResourceStringIds::OpenRecordingFolder()]);

		long itemIndex = GetSelectedStationIndex();

		if(itemIndex != -1)
		{
			const Station& station = GetStation(itemIndex);
			wxString menuItemTextTemplate = resourceStrings_[ResourceStringIds::OpenRecordingFolderForStation()];
			wxString menuItemText = wxString::Format(menuItemTextTemplate, station.GetName());
			menu.Append(ID_MENUITEM_OPENRECORDINGFOLDERFORSTATION, menuItemText);
		}

		menu.Append(ID_MENUITEM_RECORDALLSTATIONS, "Record all stations");

		PopupMenu(&menu, ScreenToClient(wxGetMousePosition()));
	}

	void menu_OnQuit(wxMenuEvent&)
	{
		pEventHandler_->OnQuit();
	}

	void menu_OnOpenRecordingFolder(wxMenuEvent&)
	{
		pEventHandler_->OnOpenRecordingFolder();
	}

	void menu_OnOpenRecordingFolderForStation(wxMenuEvent&)
	{
		long itemIndex = GetSelectedStationIndex();

		if(itemIndex == -1)
			return;

		const Station& station = GetStation(itemIndex);
		pEventHandler_->OnOpenRecordingFolder(station);
	}

	void menu_OnRecordAllStations(wxMenuEvent&)
	{
		pEventHandler_->OnRecordAllStation(stations_);
	}

	long GetSelectedStationIndex() const
	{
		return pListCtrl_->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	}

	int GetStationIdByItemIndex(long itemIndex)
	{
		return pListCtrl_->GetItemData(itemIndex);
	}

	const Station& GetStation(long itemIndex)
	{
		return stations_[static_cast<std::size_t>(itemIndex)];
	}

	long GetItemIndexByStationId(int stationId) const
	{
		typedef std::vector<Station>::const_iterator StationIterator;

		StationIterator begin = stations_.begin();
		StationIterator end = stations_.end();

		for(StationIterator current = begin; current != end; ++current)
		{
			if(current->GetId() == stationId)
				return current - begin;
		}

		return -1; 
	}
private:
	wxPanel* pPanel_;
	wxListCtrl* pListCtrl_;
	wxButton* pButtonStationDatabase_;
	wxButton* pButtonRecord_;
	wxButton* pButtonRecordStop_;
	wxButton* pButtonPlay_;
	wxButton* pButtonDelete_;

	const ResourceStrings& resourceStrings_;
	std::vector<Station> stations_;
	TEventHandler* pEventHandler_;

	enum ID
	{
		ID_MENUITEM_QUIT,
		ID_MENUITEM_OPENRECORDINGFOLDER,
		ID_MENUITEM_OPENRECORDINGFOLDERFORSTATION,
		ID_MENUITEM_RECORDALLSTATIONS
	};
};

#endif
