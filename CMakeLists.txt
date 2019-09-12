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

#-------------------------------------------------------------------------------
#
# Dependancy: qt
#
#-------------------------------------------------------------------------------

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Xml REQUIRED)

#-------------------------------------------------------------------------------
#
# Dependancy: GDAL
#
#-------------------------------------------------------------------------------

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

target_link_libraries(gpsinfo_create 
	Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml 
	${GDAL_LIBRARY} 
)
