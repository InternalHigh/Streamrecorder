#ifndef EVENTHANDLER_HPP_INCLUDED
#define EVENTHANDLER_HPP_INCLUDED

#include <string>
#include <wx/app.h>
#include <wx/event.h>
#include "Metadata.hpp"

const wxEventType EVT_CONNECTING = wxNewEventType();
const wxEventType EVT_RECORDING_STARTED = wxNewEventType();
const wxEventType EVT_RECORDING_FINISHED = wxNewEventType();
const wxEventType EVT_RECORDING_ERROR = wxNewEventType();
const wxEventType EVT_TITLE_UPDATE = wxNewEventType();

class StationEvent : public wxCommandEvent
{
public:
	StationEvent(wxEventType eventType, int stationId)
		: wxCommandEvent(eventType)
	{
		stationId_ = stationId;
	}

	virtual wxEvent* Clone() const
	{
		return new StationEvent(GetEventType(), stationId_);
	}

	int GetStationId() const
	{
		return stationId_;
	}
private:
	int stationId_;
};

class TitleUpdateEvent : public StationEvent
{
public:
	TitleUpdateEvent(int stationId, const std::string& title)
		: StationEvent(EVT_TITLE_UPDATE, stationId)
		, title_(title)
	{
	}

	virtual wxEvent* Clone() const
	{
		return new TitleUpdateEvent(GetStationId(), title_);
	}

	const std::string& GetTitle() const
	{
		return title_;
	}
private:
	std::string title_;
};

class RecordingErrorEvent : public StationEvent
{
public:
	RecordingErrorEvent(int stationId, const RecordingError& recordingError)
		: StationEvent(EVT_RECORDING_ERROR, stationId)
		, recordingError_(recordingError)
	{
	}

	virtual wxEvent* Clone() const
	{
		return new RecordingErrorEvent(GetStationId(), recordingError_);
	}

	const RecordingError& GetRecordingError() const
	{
		return recordingError_;
	}
private:
	RecordingError recordingError_;
};

class EventHandler
{
public:
	EventHandler(int stationId)
		: stationId_(stationId)
	{
	}

	void OnTitleUpdate(const std::string& title) const
	{
		TitleUpdateEvent titleUpdateEvent(stationId_, title);
		wxPostEvent(wxTheApp, titleUpdateEvent);
	}

	void OnConnecting()
	{
		StationEvent stationEvent(EVT_CONNECTING, stationId_);
		wxPostEvent(wxTheApp, stationEvent);
	}

	void OnRecordingStarted()
	{
		StationEvent stationEvent(EVT_RECORDING_STARTED, stationId_);
		wxPostEvent(wxTheApp, stationEvent);
	}

	void OnRecordingFinished()
	{
		StationEvent stationEvent(EVT_RECORDING_FINISHED, stationId_);
		wxPostEvent(wxTheApp, stationEvent);
	}

	void OnRecordingError(const RecordingError& recordingError) const
	{
		RecordingErrorEvent recordingErrorEvent(stationId_, recordingError);
		wxPostEvent(wxTheApp, recordingErrorEvent);
	}
private:
	int stationId_;
};

#endif