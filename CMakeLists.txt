cmake_minimum_required(VERSION 3.6)
project(PeopleTracker_demo)

set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp PersonTrack.cpp PersonTrack.h munkres.cpp munkres.h HungaryAlgo.cpp HungaryAlgo.h)
add_executable(PeopleTracker_demo ${SOURCE_FILES})

target_link_libraries( PeopleTracker_demo ${OpenCV_LIBS} )