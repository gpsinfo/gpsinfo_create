#-------------------------------------------------------------------------------
#
# (c) 2019 Rechenraum GmbH (office@rechenraum.com)
#
# This file is part of gpsinfo (www.gpsinfo.org).
#
# gpsinfo is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# gpsinfo is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gpsinfo. If not, see <http://www.gnu.org/licenses/>.
#
# 
#
# This file is based on the examples at https://doc.qt.io/qt-5/cmake-manual.html
#
#-------------------------------------------------------------------------------

# We use CMAKE_AUTOUIC_SEARCH_PATHS
cmake_minimum_required(VERSION 3.9.0)

project(gpsinfo_create)

set(GPSINFO_CREATE_VERSION "1.0.0")

# Rename 'local_config_EXAMPLE.cmake' to 'local_config.cmake'
include(./local_config.cmake)

#-------------------------------------------------------------------------------
#
# Dependancy: qt
#
#-------------------------------------------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (WIN32 AND QT5_PATH)
	set (CMAKE_PREFIX_PATH ${QT5_PATH})
endif (WIN32 AND QT5_PATH)

find_package(Qt5 COMPONENTS Widgets Core Gui Xml REQUIRED)

if (WIN32 AND QT5_PATH)
	set (CMAKE_PREFIX_PATH)
endif (WIN32 AND QT5_PATH)

#-------------------------------------------------------------------------------
#
# Dependancy: GDAL
#
#-------------------------------------------------------------------------------

if (GDAL_DIR)
	# findGDAL expects and environment variable
	set( ENV{GDAL_DIR} ${GDAL_DIR} )
endif (GDAL_DIR)
find_package(GDAL REQUIRED)

if (GDAL_FOUND)
	include_directories( ${GDAL_INCLUDE_DIR} )
endif (GDAL_FOUND)

#-------------------------------------------------------------------------------
#
# Source, header and UI files
#
#-------------------------------------------------------------------------------

set(GPSINFO_CREATE_SOURCES
	src/main.cpp 
	src/gpsinfoMainDialog.cpp
)

set(GPSINFO_CREATE_HEADERS
	src/gpsinfoMainDialog.h
)

set(GPSINFO_CREATE_UI
	src/gpsinfoMainDialog.ui
)

#-------------------------------------------------------------------------------
#
# Executable
#
#-------------------------------------------------------------------------------

add_executable(gpsinfo_create
    ${GPSINFO_CREATE_SOURCES}
    ${GPSINFO_CREATE_HEADERS}
	${GPSINFO_CREATE_UI}
)

# This disables the console on Windows
if (WIN32)
	set_property(TARGET gpsinfo_create PROPERTY WIN32_EXECUTABLE true)
	message(STATUS "Disabling console window ...")
endif (WIN32)

target_link_libraries(gpsinfo_create 
	Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml 
	${GDAL_LIBRARY} 
)

#-------------------------------------------------------------------------------
#
# "Installation"
#
#-------------------------------------------------------------------------------

SET(GPSINFO_CREATE_INNOSETUP installer/innosetup/gpsinfo_create.iss)
configure_file("${GPSINFO_CREATE_INNOSETUP}.in" "${CMAKE_SOURCE_DIR}/${GPSINFO_CREATE_INNOSETUP}")

if (WIN32 AND GPSINFO_CREATE_INSTALL_PATH)
	
	set(GPSINFO_CREATE_INSTALL_PATH_RELEASE "${GPSINFO_CREATE_INSTALL_PATH}/release-x64")
	set(GPSINFO_CREATE_INSTALL_PATH_DEBUG "${GPSINFO_CREATE_INSTALL_PATH}/debug-x64")

    install(TARGETS gpsinfo_create
		CONFIGURATIONS Release
		RUNTIME DESTINATION "${GPSINFO_CREATE_INSTALL_PATH_RELEASE}"
		LIBRARY DESTINATION "${GPSINFO_CREATE_INSTALL_PATH_RELEASE}"
		ARCHIVE DESTINATION "${GPSINFO_CREATE_INSTALL_PATH_RELEASE}"
	)	
    install(TARGETS gpsinfo_create
		CONFIGURATIONS Debug
		RUNTIME DESTINATION "${GPSINFO_CREATE_INSTALL_PATH_DEBUG}"
		LIBRARY DESTINATION "${GPSINFO_CREATE_INSTALL_PATH_DEBUG}"
		ARCHIVE DESTINATION "${GPSINFO_CREATE_INSTALL_PATH_DEBUG}"
	)
endif ()
