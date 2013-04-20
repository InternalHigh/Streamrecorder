#ifndef SHOUTCASTSTREAMHANDLER_HPP_INCLUDED
#define SHOUTCASTSTREAMHANDLER_HPP_INCLUDED

#include "Metadata.hpp"
#include "StreamWriter.hpp"
#include "EventHandler.hpp"

class ShoutcastStreamHandler
{
public:
	ShoutcastStreamHandler(StreamWriter& streamWriter, const EventHandler& eventHandler)
		: streamWriter_(streamWriter)
		, eventHandler_(eventHandler)
	{
	}

	void ProcessAudioData(const char* pBuffer, std::size_t bufferSize)
	{
		streamWriter_.WriteBuffer(pBuffer, bufferSize);
	}

	void ProcessMetadata(const Metadata& metadata) const
	{
		eventHandler_.OnTitleUpdate(metadata.GetTitle());
	}
private:
	ShoutcastStreamHandler(const ShoutcastStreamHandler&);
	ShoutcastStreamHandler& operator=(const ShoutcastStreamHandler&);
private:
	StreamWriter& streamWriter_;
	const EventHandler& eventHandler_;
};

#endif
