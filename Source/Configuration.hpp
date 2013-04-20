#ifndef CONFIGURATION_HPP_INCLUDED
#define CONFIGURATION_HPP_INCLUDED

#include <string>

class Configuration
{
public:
	void SetPlayerPath(const std::string& playerPath)
	{
		playerPath_ = playerPath;
	}

	std::string GetPlayerPath() const
	{
		return playerPath_;
	}

	void SetSavePath(const std::string& savePath)
	{
		savePath_ = savePath;
	}

	std::string GetSavePath() const
	{
		return savePath_;
	}
private:
	std::string playerPath_;
	std::string savePath_;
};



#endif
