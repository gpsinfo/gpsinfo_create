if (WIN32)
	# Download from
	#	https://www.qt.io/
	set(QT5_PATH "C:/Qt/Qt5.12.12/5.12.12/msvc2017_64")

	# Download 
	#	https://download.gisinternals.com/sdk/downloads/release-1916-x64-gdal-3-4-2-mapserver-7-6-4.zip
	# and
	#	https://download.gisinternals.com/sdk/downloads/release-1928-x64-gdal-3-4-2-mapserver-7-6-4-libs.zip
	# and unpack to a folder named 'gdal-3.4.2-vs2019-x64'
	set(GDAL_DIR "$ENV{UserProfile}/Desktop/src/gdal-3.4.2-vs2019-x64")

	# Shall point to the directory with the exe and its deps
	set(GPSINFO_CREATE_INSTALL_PATH "$ENV{UserProfile}/Desktop/gpsinfo_create_env")
	# The setup package is written to this directory
	set(GPSINFO_CREATE_INSTALLER_OUTPUT "$ENV{UserProfile}/Desktop/gpsinfo_create_env/installer")
endif (WIN32)
