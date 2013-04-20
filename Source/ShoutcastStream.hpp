#ifndef SHOUTCASTSTREAM_HPP
#define SHOUTCASTSTREAM_HPP

#include <string>
#include <stdexcept>

#include "ShoutcastStreamHandler.hpp"
#include "MetadataParser.hpp"

template<class HandlerType>
class ShoutcastStream
{
public:
	ShoutcastStream(HandlerType& shoutcastStreamHandler)
		: metaDataInterval_(0)
		, shoutcastStreamHandler_(shoutcastStreamHandler)
	{
		Reset();
	}

	void ProcessBuffer(const char* pBuffer, std::size_t bufferSize)
	{
		const char* pBufferEnd = pBuffer + bufferSize;

		while(pBuffer != pBufferEnd)
		{
			pBuffer += ProcessBufferChunk(pBuffer, pBufferEnd - pBuffer);
		}
	}

	void SetMetadataInterval(std::size_t metadataInterval)
	{
		metaDataInterval_ = metadataInterval;
		Reset();
	}
private:
	std::size_t ProcessBufferChunk(const char* pBuffer, std::size_t bufferSize)
	{
		switch(state_)
		{
		case AUDIODATA:
			return ProcessAudioDataChunk(pBuffer, bufferSize);
		case METADATA_LENGTH:
			return ProcessMetadataLengthChunk(pBuffer, bufferSize);
		case METADATA:
			return ProcessMetadataChunk(pBuffer, bufferSize);
		default:
			throw std::runtime_error("Invalid state");
		}
	}

	std::size_t ProcessAudioDataChunk(const char* pBuffer, std::size_t bufferSize)
	{
		if(metaDataInterval_ == 0)
		{
			ProcessAudioData(pBuffer, bufferSize);
			return bufferSize;
		}

		std::size_t bytesToProcess = std::min(bufferSize, bytesRemaining_);

		ProcessAudioData(pBuffer, bytesToProcess);

		bytesRemaining_ -= bytesToProcess;

		if(bytesRemaining_ == 0)
		{
			state_ = METADATA_LENGTH;
			bytesRemaining_ = 1;
		}

		return bytesToProcess;
	}

	std::size_t ProcessMetadataLengthChunk(const char* pBuffer, std::size_t bufferSize)
	{
		assert(bufferSize > 0);

		std::size_t numberOfBlocks = pBuffer[0];
		bytesRemaining_ = numberOfBlocks * BLOCK_SIZE;

		if(bytesRemaining_ == 0)
		{
			Reset();
		}
		else
		{
			state_ = METADATA;
		}

		return 1;
	}

	std::size_t ProcessMetadataChunk(const char* pBuffer, std::size_t bufferSize)
	{
		std::size_t bytesToProcess = std::min(bufferSize, bytesRemaining_);

		metadataBuffer_ += std::string(pBuffer, pBuffer + bytesToProcess);

		bytesRemaining_ -= bytesToProcess;

		if(bytesRemaining_ == 0)
		{
			Metadata metadata = MetadataParser::Parse(metadataBuffer_);

			ProcessMetadata(metadata);

			Reset();
		}

		return bytesToProcess;
	}

	void ProcessAudioData(const char* pBuffer, std::size_t bufferSize) const
	{
		shoutcastStreamHandler_.ProcessAudioData(pBuffer, bufferSize);
	}

	void ProcessMetadata(const Metadata& metadata) const
	{
		shoutcastStreamHandler_.ProcessMetadata(metadata);
	}
private:
	void Reset()
	{
		state_ = AUDIODATA;
		bytesRemaining_ = metaDataInterval_;
		metadataBuffer_.clear();
	}
private:
	enum State
	{
		AUDIODATA,
		METADATA_LENGTH,
		METADATA
	} state_;

	static const std::size_t BLOCK_SIZE = 16;

	std::size_t bytesRemaining_;
	std::string metadataBuffer_;

	std::size_t metaDataInterval_;
	HandlerType& shoutcastStreamHandler_;
};

#endif