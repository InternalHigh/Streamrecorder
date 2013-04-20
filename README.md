![Streamrecorder](http://home.arcor.de/grober/Streamrecorder.png)

#[Download](http://home.arcor.de/grober/Streamrecorder%200.8.zip)

How to build with Visual Studio
-------------------------------

- Extract "Dependencies.7z" to folder "Dependencies"
- Open "Visual Studio Command Prompt" and execute "Build\BuildDependencies_VC.bat"
- Open "Build\Streamrecorder.sln"
- "Build" => "Build Solution"

How to build with MinGW
-----------------------

- Install MinGW: http://tdm-gcc.tdragon.net
- Install CMake: http://www.cmake.org
- Extract "Dependencies.7z" to folder "Dependencies"
- Execute "Build\BuildDependencies_MinGW.bat"
- Start "CMake (cmake-gui)", set the "Build" folder as the source code location, specify an output directory and generate "MinGW Makefiles"
- Open a command prompt and switch to the specified output directory
- Call "mingw32-make"

How to build on Linux
---------------------

For systems where APT is available:

- ./Build/Build.sh

Tested on Linux Mint 14 and Ubuntu 12.10.
