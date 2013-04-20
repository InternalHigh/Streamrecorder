sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libgtk2.0-dev
sudo apt-get install cmake
sudo apt-get install p7zip-full
cd $(dirname $0)/..
7z x Dependencies.7z -oDependencies
cd Dependencies/wxWidgets/
./configure
make
sudo make install
cd ../libcurl/
./configure
make
sudo make install
cd ../../Build/
cmake .
make

