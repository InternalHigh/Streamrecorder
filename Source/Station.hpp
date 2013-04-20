#ifndef STATION_HPP_INCLUDED
#define STATION_HPP_INCLUDED

#include <string>

class Station
{
public:
	Station(int id,
	        const std::string& name,
	        const std::string& homepageUrl,
	        const std::string& streamUrl)
		: id_(id)
		, name_(name)
		, homepageUrl_(homepageUrl)
		, streamUrl_(streamUrl)
	{
	}

	int GetId() const
	{
		return id_;
	}

	void SetId(int id)
	{
		id_ = id;
	}

	const std::string& GetName() const
	{
		return name_;
	}

	const std::string& GetHomepageUrl() const
	{
		return homepageUrl_;
	}

	const std::string& GetStreamUrl() const
	{
		return streamUrl_;
	}
private:
	int id_;
	std::string name_;
	std::string homepageUrl_;
	std::string streamUrl_;
};

#endif