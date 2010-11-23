# CMakeLists.txt for liborigin
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
add_library (origin SHARED ${sources})
set_target_properties(origin PROPERTIES VERSION 2.0.0 SOVERSION 2 )

# static library
add_library (origin-static STATIC ${sources})
set_target_properties(origin-static PROPERTIES OUTPUT_NAME "origin")

# install libraries
install(TARGETS origin origin-static DESTINATION lib)

# install headers
install(FILES ${devel-headers} DESTINATION include)

# command line util
add_executable(opj2dat opj2dat.cpp)
target_link_libraries (opj2dat origin-static)

install(TARGETS opj2dat DESTINATION bin)
