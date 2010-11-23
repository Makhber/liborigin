# CMakeLists.txt for liborigin
cmake_minimum_required(VERSION 2.8)

# boost library headers
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

# library version
set(LIBORIGIN_VERSION_MAJOR 2)
set(LIBORIGIN_VERSION_MINOR 0)
set(LIBORIGIN_VERSION_BUGFIX 0)

# compile-time configuration variables to be linked in
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

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
	${CMAKE_CURRENT_BINARY_DIR}/config.h
	OriginObj.h
	OriginFile.h
	OriginParser.h
	tree.hh
	)

# dynamic library
add_library (origin SHARED ${sources})
set_target_properties(origin PROPERTIES
	VERSION ${LIBORIGIN_VERSION_MAJOR}.${LIBORIGIN_VERSION_MINOR}.${LIBORIGIN_VERSION_BUGFIX}
	SOVERSION ${LIBORIGIN_VERSION_MAJOR} )

# static library
add_library (origin-static STATIC ${sources})
set_target_properties(origin-static PROPERTIES OUTPUT_NAME "origin")

# install libraries
install(TARGETS origin origin-static DESTINATION lib)

# install headers
install(FILES ${devel-headers} DESTINATION include/liborigin)

# command line util
add_executable(opj2dat opj2dat.cpp)
target_link_libraries (opj2dat origin-static)

install(TARGETS opj2dat DESTINATION bin)

# documentation
configure_file(doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
