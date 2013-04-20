#ifndef METADATAPARSER_HPP_INCLUDED
#define METADATAPARSER_HPP_INCLUDED

#include "Metadata.hpp"
#include <string>

class MetadataParser
{
public:
	static bool ParseMetadataIntervalHeader(const std::string& headerLine, std::size_t& metadataInterval)
	{
		std::string key;
		std::string value;

		if(!SplitHeaderLine(headerLine, key, value))
			return false;

		if(key != "icy-metaint")
			return false;

		std::string metaIntervalString = value;

		if(metaIntervalString.empty() || !StringContainsOnlyDigits(metaIntervalString))
			return false;

		char* pEnd;
		metadataInterval = strtoul(metaIntervalString.c_str(), &pEnd, 10);

		if(metadataInterval == ULONG_MAX && errno == ERANGE)
			return false;

		return pEnd == metaIntervalString.c_str() + metaIntervalString.length();
	}

	static Metadata Parse(const std::string& metadataBuffer) 
	{
		std::string searchString = "StreamTitle='";
		size_t streamTitleStringPosition = metadataBuffer.find(searchString);

		size_t titleBegin = streamTitleStringPosition + searchString.length();

		size_t titleEnd = metadataBuffer.find("';", titleBegin);

		std::string title = metadataBuffer.substr(titleBegin, titleEnd - titleBegin);

		return Metadata(title);
	}
private:
	static bool SplitHeaderLine(const std::string& headerLine, std::string& key, std::string& value)
	{
		std::size_t colonPosition = headerLine.find(':');

		if(colonPosition == std::string::npos)
			return false;

		key = headerLine.substr(0, colonPosition);

		std::size_t valueBegin = headerLine.find_first_not_of(" ", colonPosition + 1);
		std::size_t valueEnd = headerLine.find_last_not_of("\r\n");

		value = headerLine.substr(valueBegin, valueEnd - valueBegin + 1);

		return true;
	}

	static bool StringContainsOnlyDigits(const std::string& inputString)
	{
		return inputString.find_first_not_of("0123456789") == std::string::npos;
	}
};

#endif
