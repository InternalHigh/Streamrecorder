#ifndef METADATA_HPP_INCLUDED
#define METADATA_HPP_INCLUDED

#include <string>

class Metadata
{
public:
	Metadata(const std::string& title)
		: title_(title)
	{
	}

	const std::string& GetTitle() const
	{
		return title_;
	}
private:
	std::string title_;
};

#endif