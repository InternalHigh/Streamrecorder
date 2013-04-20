#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <string>

#include <wx/utils.h>

#include "Configuration.hpp"
#include "Station.hpp"

class PlayerErrorCode
{
public:
	enum Value
	{
		PlayerNotAvailable,
		PlayerExecutionFailed
	};
};

class PlayerError
{
public:
	PlayerError(PlayerErrorCode::Value errorCode, const std::string& message)
		: errorCode_(errorCode)
		, message_(message)
	{
	}

	PlayerErrorCode::Value GetErrorCode() const
	{
		return errorCode_;
	}

	const std::string& GetMessage() const
	{
		return message_;
	}
private:
	PlayerErrorCode::Value errorCode_;
	std::string message_;
};

class Player
{
public:
	Player(const std::string& playerPath)
		: playerPath_(playerPath)
	{
	}

	void Play(const Station& station) const
	{	
		if(!IsPlayerAvailable())
			throw PlayerError(PlayerErrorCode::PlayerNotAvailable, playerPath_);

		std::string commandString = playerPath_ + " " + GetStreamUrl(station);
		
		wxLogNull suppressErrorMessages;

		long processId = wxExecute(commandString);

		if(processId == 0)
			throw PlayerError(PlayerErrorCode::PlayerExecutionFailed, commandString);
	}
private:
	bool IsPlayerAvailable() const
	{
		if(wxFileExists(playerPath_))
			return true;

		wxPathList pathList;
		pathList.AddEnvList("PATH");
		wxString path = pathList.FindValidPath(playerPath_);
		return !path.IsEmpty();
	}

	static std::string GetStreamUrl(const Station& station)
	{
		std::string httpPrefix = "http://";

		const std::string& streamUrl = station.GetStreamUrl();

		if(streamUrl.compare(0, httpPrefix.length(), httpPrefix) != 0)
			return httpPrefix + streamUrl;

		return streamUrl;
	}

	std::string playerPath_;
};

#endif