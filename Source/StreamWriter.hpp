#ifndef STREAMWRITER_HPP_INCLUDED
#define STREAMWRITER_HPP_INCLUDED

#include <fstream>
#include <cerrno>
#include <cstring>

#include "RecordingError.hpp"

class StreamWriter
{
public:
	void OpenFile(const std::string& filePath)
	{
		fileOutputStream_.open(filePath.c_str(), std::ios::binary);
		
		if(!fileOutputStream_.good())
			ThrowRecordingError(RecordingErrorCode::FileOpenFailed);
	}

	void WriteBuffer(const char* pBuffer, std::size_t bufferSize)
	{
		fileOutputStream_.write(pBuffer, bufferSize);
		
		if(!fileOutputStream_.good())
			ThrowRecordingError(RecordingErrorCode::FileWriteFailed);
	}
private:
	static void ThrowRecordingError(RecordingErrorCode::Value recordingErrorCode)
	{
		std::string errorMessage = std::strerror(errno);
		throw RecordingError(recordingErrorCode, errorMessage);
	}
private:
	std::ofstream fileOutputStream_;
};

#endif