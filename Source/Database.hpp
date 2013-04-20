#ifndef DATABASE_HPP_INCLUCED
#define DATABASE_HPP_INCLUCED

#include <string>
#include <sstream>
#include <vector>

#include <sqlite3.h>

#include "Station.hpp"

class Database
{
public:
	explicit Database(const std::string& filename)
	{
		sqlite3_open(filename.c_str(), &pDatabase_);
	}

	~Database()
	{
		sqlite3_close(pDatabase_);
	}

	std::vector<Station> GetStationsNotOnPlaylist() const
	{
		std::vector<Station> stations;

		std::string query = "SELECT"
		                    " id,"
		                    " name,"
		                    " homepageUrl,"
		                    " streamUrl"
		                    " FROM Stations WHERE id NOT IN"
		                    " (SELECT id from SelectedStations)"
		                    " ORDER BY name;";

		sqlite3_exec(pDatabase_, query.c_str(), GetStationsCallback, &stations, NULL);

		return stations;
	}

	std::vector<Station> GetStationsOnPlaylist() const
	{
		std::vector<Station> stations;

		std::string query = "SELECT"
		                    " Stations.id,"
		                    " Stations.name,"
		                    " Stations.homepageUrl,"
		                    " Stations.streamUrl"
		                    " FROM SelectedStations"
		                    " INNER JOIN Stations ON SelectedStations.id = Stations.id;";

		sqlite3_exec(pDatabase_, query.c_str(), GetStationsCallback, &stations, NULL);

		return stations;
	}

	void AddStationToPlaylist(const Station& station) const
	{
		char* pQuery = sqlite3_mprintf("INSERT INTO SelectedStations(id) VALUES(%i)", station.GetId());

		ExecuteQuery(pQuery);

		sqlite3_free(pQuery);
	}

	void AddStation(Station& station) const
	{
		char* pQuery = sqlite3_mprintf("INSERT INTO Stations(name, homepageUrl, streamUrl) VALUES('%q', '%q', '%q')",
		                               station.GetName().c_str(),
		                               station.GetHomepageUrl().c_str(),
		                               station.GetStreamUrl().c_str());

		ExecuteQuery(pQuery);

		sqlite3_free(pQuery);

		int stationId = sqlite3_last_insert_rowid(pDatabase_);

		station.SetId(stationId);
	}

	void RemoveStationFromPlaylist(int stationId) const
	{
		char* pQuery = sqlite3_mprintf("DELETE FROM SelectedStations WHERE id = %i", stationId);

		ExecuteQuery(pQuery);

		sqlite3_free(pQuery);
	}
private:
	void ExecuteQuery(const char* pQuery) const
	{
		sqlite3_exec(pDatabase_, pQuery, NULL, NULL, NULL);
	}

	static int GetStationsCallback(void* userData, int, char** argv, char**)
	{
		std::vector<Station>* pStations = static_cast<std::vector<Station>*>(userData);

		Station station(atoi(argv[0]), argv[1], argv[2], argv[3]);

		pStations->push_back(station);

		return 0;
	}

	Database(const Database&);
	Database& operator=(const Database&);
private:
	sqlite3* pDatabase_;
};

#endif