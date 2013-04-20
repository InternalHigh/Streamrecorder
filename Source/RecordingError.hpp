#ifndef RECORDINGERROR_HPP_INCLUDED
#define RECORDINGERROR_HPP_INCLUDED

#include <string>

class RecordingErrorCode
{
public:
	enum Value
	{
		ConnectFailed,
		FileOpenFailed,
		FileWriteFailed,
		InvalidStream
	};
};

class RecordingError
{
public:
	RecordingError(RecordingErrorCode::Value errorCode, const std::string& errorMessage)
		: errorCode_(errorCode)
		, errorMessage_(errorMessage)
	{
	}

	RecordingErrorCode::Value GetErrorCode() const
	{
		return errorCode_;
	}

	const std::string& GetErrorMessage() const
	{
		return errorMessage_;
	}
private:
	RecordingErrorCode::Value errorCode_;
	std::string errorMessage_;
};

#endif