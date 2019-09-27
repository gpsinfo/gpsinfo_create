Download the "Microsoft Visual C++ [YEAR] Redistributable Update [UPDATE]" for
	the employed compiler, e.g. YEAR=2017, UPDATE=3. We currently deploy a 
	64-bit application only, hence we need to download vc_redist.x64 only.
	Store the downloaded file to the path referenced in the Inno Setup 
	configuration file.

Make sure that the correct depending DLLs (all except qt, see next step) are
located in the release_x64 folder: gdal204.dll & Co (unpack 
	http://download.gisinternals.com/sdk/downloads/release-1911-x64-gdal-2-4-2-mapserver-7-4-0.zip
to gdal-2.4.2-vs2017-x64 and copy the entire bin folder contents to the 
directory storing gpsinfo_create.exe)

Follow http://doc.qt.io/qt-5/windows-deployment.html to obtain all qt 
dependencies. In VS2017's developer command line prompt, run
	C:\Qt\5.12.4\msvc2017_64\bin\windeployqt.exe --no-compiler-runtime --no-translations --no-quick-import --no-system-d3d-compiler --no-webkit2 ./gpsinfo_create.exe
	
