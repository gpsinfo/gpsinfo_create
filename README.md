# gpsinfo_create - Developer Documentation

A user friendly, GPL3 licensed tile creator for WMTS/gpsinfo based services. See 
http://www.gpsinfo.org for more information.

## Compilation on Windows (Visual Studio 2019)

gpsinfo_create depends on
* Qt 5 (https://www.qt.io/)
* GDAL (We recomment to use the prebuild packaged GISInternals, 
	https://download.gisinternals.com/sdk/downloads/release-1916-x64-gdal-3-4-2-mapserver-7-6-4.zip and
	https://download.gisinternals.com/sdk/downloads/release-1928-x64-gdal-3-4-2-mapserver-7-6-4-libs.zip)

At the time of writing this document, both dependencies were available as 
Visual Studio 2019 prebuilt binaries. Hence, we recomment to build 
gpsinfo_create in 64-bit with Visual Studio 2019.

We use the cmake build system (https://cmake.org/), that allows to generate a 
Visual Studio solution from the accompanied CMakeLists.txt. Copy 
local_config_EXAMPLE.config in the source tree's root to local_config.cmake
and edit the latter to provide the paths to both Qt and gdal.

### Deployment

These are instructions for a simple manual deployment on windows

* Compile a 64-bit executable of gpsinfo_create with Visual Studio 2019.
* Copy the generated gpsinfo_create.exe executable to an empty directory that we
	refer to as DIR in the following.
* Open the 'Developer Command Prompt for Visual Studio 2019' and change to DIR.
* In order to add all necessary Qt runtime files to DIR, run
		C:\Qt\Qt5.12.12\5.12.12\msvc2017_64\bin\windeployqt.exe --no-compiler-runtime --no-translations --no-quick-import --no-system-d3d-compiler --no-webkit2 gpsinfo_create.exe
* Copy the contents of gdal-3.4.2-vs2019-x64\bin to DIR
* Distribute DIR

## Compilation on Linux (Ubuntu et al.)

Compilation on Linux is straight forward. Install the development packages
of QT 5 and gdal, generate Makefiles with cmake (create local_config.cmake as 
described in above section windows compilation) and run them.

## Further Documentation

The source code is documented in doxygen style format.

Visit https://www.gpsinfo.org for further documentation on WMTS-based elevation
services and https://wmts.rechenraum.com for a reference service.

## Acknowledgements

In 2019 and 2021, development of gpsinfo_create received funding from 
Internet Foundation  Austria within netidee 2018 and 2020, respectively. 
