#ifndef STATIONIDTORECORDERMAP_HPP_INCLUDED
#define STATIONIDTORECORDERMAP_HPP_INCLUDED

#include <map>
#include "Recorder.hpp"

class StationIdToRecorderMap
{
private:
	typedef std::map<int, Recorder*> Map;
public:
	typedef Map::const_iterator Iterator;

	void Add(int stationId, Recorder* pRecorder)
	{
		assert(map_.find(stationId) == map_.end());
		map_[stationId] = pRecorder;
	}

	Iterator GetBeginIterator()
	{
		return map_.begin();
	}

	Iterator GetEndIterator()
	{
		return map_.end();
	}

	void Remove(int stationId)
	{
		map_.erase(stationId);
	}

	Recorder* GetRecorder(int stationId)
	{
		Iterator iter = map_.find(stationId);

		if(iter == map_.end())
			return NULL;

		return iter->second;
	}

	bool IsEmpty() const
	{
		return map_.empty();
	}
private:
	Map map_;
};

#endif
