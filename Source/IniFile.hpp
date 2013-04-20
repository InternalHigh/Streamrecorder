#ifndef INIFILE_HPP_INCLUDED
#define INIFILE_HPP_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

class IniFile
{
public:
	IniFile(std::istream& inputStream)
	{
		std::string line;
		while(getline(inputStream, line))
		{
			lines_.push_back(line);
		}
	}

	std::string ReadString(const std::string& sectionName, const std::string& key) const
	{
		std::string result;
		ReadValue(sectionName, key, result);
		return result;
	}

	bool ReadValue(const std::string& sectionName, const std::string& key, std::string& value) const
	{
		size_t position;
		bool found = Find(sectionName, key, position);

		if(!found)
			return false;

		const std::string& line = lines_[position];

		ExtractValue(line, value);

		return true;
	}
private:
	bool FindSection(const std::string& sectionName, size_t& position) const
	{
		std::string sectionString = CreateSectionString(sectionName);

		position = 0;

		for(; position < lines_.size(); ++position)
		{
			const std::string& line = lines_[position];

			if(line == sectionString)
				return true;
		}

		return false;
	}

	bool FindKeyInSection(std::size_t sectionPosition,
	                      const std::string& key,
	                      std::size_t& keyPosition) const
	{
		keyPosition = sectionPosition + 1;

		for(; keyPosition < lines_.size(); ++keyPosition)
		{
			const std::string& line = lines_[keyPosition];

			if(IsSection(line))
				break;

			std::string currentKey;
			ExtractKey(line, currentKey);

			if(currentKey == key)
				return true;
		}

		return false;
	}

	bool Find(const std::string& sectionName,
	          const std::string& key,
	          size_t& position) const
	{
		size_t sectionPosition;
		bool sectionFound = FindSection(sectionName, sectionPosition);

		if(!sectionFound)
			return false;

		bool keyFound = FindKeyInSection(sectionPosition, key, position);

		if(!keyFound)
			return false;

		return true;
	}

	static void ExtractKey(const std::string& str, std::string& key)
	{
		std::string::size_type seperatorPosition = str.find(keyValueSeperator);

		key = std::string(str, 0, seperatorPosition);
	}

	static void ExtractValue(const std::string& str, std::string& value)
	{
		std::string::size_type seperatorPosition = str.find(keyValueSeperator);

		value = std::string(str, seperatorPosition + 1, str.length() - seperatorPosition);
	}

	static std::string CreateSectionString(const std::string& sectionName)
	{
		return '[' + sectionName + ']';
	}

	static bool IsSection(const std::string& str)
	{
		if(!str.empty() && str[0] == '[')
			return true;

		return false;
	}
private:
	static const char keyValueSeperator = '=';

	std::vector<std::string> lines_;
};

#endif