# https://doc.qt.io/qt-5/cmake-manual.html

# We use CMAKE_AUTOUIC_SEARCH_PATHS
cmake_minimum_required(VERSION 3.9.0)

project(gpsinfo_create)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets Core Gui Xml REQUIRED)

set(GPSINFO_CREATE_SOURCES
	src/main.cpp 
	src/gpsinfoMainDialog.cpp
)

set(GPSINFO_CREATE_HEADERS
	src/gpsinfoMainDialog.h
)

# The following requires cmake 3.9 and renders it unnecessary to add the UI 
# files to 'add_executable'
set(CMAKE_AUTOUIC_SEARCH_PATHS "./ui")
set(GPSINFO_CREATE_UI
	gpsinfoMainDialog.ui
)

add_executable(gpsinfo_create
    ${GPSINFO_CREATE_SOURCES}
    ${GPSINFO_CREATE_HEADERS}
)

target_link_libraries(gpsinfo_create Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml)
