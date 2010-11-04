# CMakeLists.txt for liborigin2
cmake_minimum_required(VERSION 2.8)

# boost library headers
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# source files
set (sources
	OriginFile.cpp
	OriginParser.cpp
	OriginDefaultParser.cpp
	Origin600Parser.cpp
	Origin610Parser.cpp
	Origin700Parser.cpp
	Origin750Parser.cpp
	Origin800Parser.cpp
	Origin810Parser.cpp
	)

# header files for development
set (devel-headers
	OriginObj.h
	OriginFile.h
	OriginParser.h
	tree.hh
	)

# dynamic library
add_library (origin2 SHARED ${sources})
set_target_properties(origin2 PROPERTIES VERSION 0.0.1 SOVERSION 0 )

# static library
add_library (origin2-static STATIC ${sources})
set_target_properties(origin2-static PROPERTIES OUTPUT_NAME "origin2")

# install libraries
install(TARGETS origin2 origin2-static DESTINATION lib)

# install headers
install(FILES ${devel-headers} DESTINATION include)

# command line util
add_executable(opj2dat opj2dat.cpp)
target_link_libraries (opj2dat origin2-static)

install(TARGETS opj2dat DESTINATION bin)
