# CMakeLists.txt for liborigin
cmake_minimum_required(VERSION 3.22)

project( liborigin
    VERSION 3.0.3
    LANGUAGES CXX
)

if( CMAKE_VERSION VERSION_LESS "3.21" AND "${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}" )
    set(PROJECT_IS_TOP_LEVEL TRUE)
endif()

if( PROJECT_IS_TOP_LEVEL )
  set( CMAKE_CXX_STANDARD 20 )
  set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
  set( CMAKE_CXX_EXTENSIONS OFF )
endif()

# allow for installation of individual targets
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

if( PROJECT_IS_TOP_LEVEL )
    option(BUILD_SHARED_LIBS "Build shared library" ON)
endif()
option(BUILD_STATIC_LIBS "Build static library" ON)

if( NOT BUILD_STATIC_LIBS AND NOT BUILD_SHARED_LIBS )
    message( FATAL_ERROR "You must enable building shared or static library" )
endif()

if( BUILD_SHARED_LIBS AND (MSVC OR NOT PROJECT_IS_TOP_LEVEL) )
    set( BUILD_STATIC_LIBS OFF CACHE BOOL "Build static library" FORCE )
endif()

if( PROJECT_IS_TOP_LEVEL )
    if( MSVC )
        # /wd4456 /wd4458 Silent "declaration of %1 hides %2 ..."
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4 /permissive- /wd4456 /wd4458" )
        add_compile_definitions( _CRT_SECURE_NO_WARNINGS
            _SILENCE_CXX17_OLD_ALLOCATOR_MEMBERS_DEPRECATION_WARNING )
        if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
            set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nonportable-include-path" )
        else()
            add_compile_options( "/MP" )
        endif()
    else()
        set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -pedantic -Wzero-as-null-pointer-constant" )
    endif()
endif()

# compile-time configuration variables to be linked in
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# source files
set (sources
    OriginFile.cpp
    OriginParser.cpp
    OriginAnyParser.cpp
)

# header files for development
set (devel-headers
    OriginObj.h
    OriginFile.h
    OriginParser.h
    tree.hh
)

# object library
add_library (origin OBJECT ${sources})
set_target_properties(origin PROPERTIES POSITION_INDEPENDENT_CODE ON)

# inform on log setting
option(GENERATE_CODE_FOR_LOG "Enable logging file parsing" OFF)
if (GENERATE_CODE_FOR_LOG)
    target_compile_definitions(origin PRIVATE GENERATE_CODE_FOR_LOG)
endif ()

include(GNUInstallDirs)

# Static library
if (BUILD_STATIC_LIBS)
    add_library (origin_static STATIC $<TARGET_OBJECTS:origin>)
    set_target_properties(origin_static PROPERTIES OUTPUT_NAME "origin" )
endif ()

# Shared library
if (BUILD_SHARED_LIBS)
    if (WIN32)
        target_compile_definitions( origin
            PUBLIC ORIGIN_DLL
            PRIVATE ORIGIN_DLL_EXPORT
        )
    endif()
    add_library( origin_shared SHARED $<TARGET_OBJECTS:origin>)
    set_target_properties(origin_shared PROPERTIES
        OUTPUT_NAME "origin"
    )
    install( TARGETS origin_shared
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
endif ()

if( PROJECT_IS_TOP_LEVEL )
    # command line util
    add_executable(opj2dat opj2dat.cpp)
    if (BUILD_SHARED_LIBS)
        target_link_libraries (opj2dat origin_shared)
    else ()
        target_link_libraries (opj2dat origin_static)
    endif ()

    if( BUILD_SHARED_LIBS )
        set_target_properties(origin_shared PROPERTIES
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
        )
    endif()

    if( BUILD_STATIC_LIBS )
        install( TARGETS origin_static
            LIBRARY DESTINATION ${CMAKE_INSTALL_BINDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        )
    endif()
    # Generate a pkg-config file matching this config
    configure_file("liborigin.pc.in" "liborigin.pc" @ONLY)

    # install headers
    install(FILES ${devel-headers} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/liborigin)
    # install pkg-config file
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/liborigin.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
    install(TARGETS opj2dat DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)

    # documentation
    install(FILES COPYING FORMAT README
        DESTINATION ${CMAKE_INSTALL_DOCDIR}
        OPTIONAL
    )

    # Doxygen
    configure_file(doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        MESSAGE (STATUS "Building Doxygen docu")
        add_custom_target(doc ALL ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
        set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/html)
        install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
            DESTINATION ${CMAKE_INSTALL_DOCDIR}
            OPTIONAL
        )
    endif ()
endif()
