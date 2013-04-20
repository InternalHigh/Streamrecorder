@echo off

cd /D %~dp0

sqlite3 ../Data/stations.db < schema.txt
sqlite3 ../Data/stations.db < stations.txt
