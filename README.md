# gpsinfo_create

A user friendly, GPL3 licensed tile creator for WMTS/gpsinfo based services. See 
http://www.gpsinfo.org for more information.

## Compilation on Windows (Visual Studio 2017)

gpsinfo_create depends on
* Qt 5 (https://www.qt.io/)
* gdal 2.4.2(We recomment to use the prebuild packaged GISInternals, 
	http://download.gisinternals.com/sdk/downloads/release-1911-x64-gdal-2-4-2-mapserver-7-4-0-libs.zip)

At the time of writing this document, both dependencies were available as 
Visual Studio 2017 prebuild binaries. Hence, we recomment to build 
gpsinfo_create in 64-bit with Visual Studio 2017.

We use the cmake build system (https://cmake.org/), that allows to generate a 
Visual Studio solution from the accompanied CMakeLists.txt

### Deployment

These are instructions for a simple manual deployment on windows

* Compile a 64-bit executable of gpsinfo_create with Visual Studio 2017.
* Copy the generated gpsinfo_create.exe executable to an empty directory that we
	refer to as DIR in the following.
* Open the 'Developer Command Prompt for Visual Studio 2017' and change to DIR.
* In order to add all necessary Qt runtime files to DIR, run
		C:\Qt\5.12.4\msvc2017_64\bin\windeployqt.exe --no-compiler-runtime --no-translations --no-quick-import --no-system-d3d-compiler --no-webkit2 gpsinfo_create.exe
* Copy the contents of gdal-2.4.2-vs2017-x64\bin to DIR
* Distribute DIR

## Compilation on Linux (Ubuntu et al.)

Compilation on Linux is straight forward. Install the development packages
of QT 5 and gdal, generate Makefiles with cmake and run them.
