if (WIN32)
	# Download from
	#	https://www.qt.io/
	set(QT5_PATH "C:/Qt/5.12.4/msvc2017_64")

	# Download 
	#	http://download.gisinternals.com/sdk/downloads/release-1911-x64-gdal-2-4-2-mapserver-7-4-0-libs.zip
	# and unpack to a folder named 'gdal-2.4.2-vs2017-x64'
	set(GDAL_DIR "$ENV{UserProfile}/Desktop/src/gdal-2.4.2-vs2017-x64")

	# Shall point to the directory with the exe and its deps
	set(GPSINFO_CREATE_INSTALL_PATH "$ENV{UserProfile}/Desktop/gpsinfo_create_env")
	# The setup package is written to this directory
	set(GPSINFO_CREATE_INSTALLER_OUTPUT "$ENV{UserProfile}/Desktop/gpsinfo_create_env/installer")
endif (WIN32)
