cmake_minimum_required(VERSION 3.16.3)

project( liborigin
  VERSION 3.0.1
  LANGUAGES CXX
)

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
set( CMAKE_CXX_EXTENSIONS OFF )

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
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -pedantic" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic" )
endif()

# compile-time configuration variables to be linked in
configure_file(config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

# source files
set (sources
  "OriginFile.cpp"
  "OriginParser.cpp"
  "OriginAnyParser.cpp"
  "OriginObj.h"
  "OriginFile.h"
  "OriginParser.h"
  "tree.h"
  "endianfstream.h"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

# header files for development
set (devel-headers
  "OriginObj.h"
  "OriginFile.h"
  "OriginParser.h"
  "tree.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/config.h"
)

# allow for installation of individual targets
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

# object library
add_library( origin OBJECT ${sources} )
set_target_properties( origin PROPERTIES POSITION_INDEPENDENT_CODE ON )

target_include_directories( origin PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_include_directories( origin PRIVATE ${CMAKE_CURRENT_BINARY_DIR} )

option( LIBORIGIN_GENERATE_CODE_FOR_LOG "Log File parsing" OFF )
if( LIBORIGIN_GENERATE_CODE_FOR_LOG )
  target_compile_definitions( origin PUBLIC GENERATE_CODE_FOR_LOG )
endif()

include(GNUInstallDirs)

option( LIBORIGIN_STATIC_LIB "Build Static Library" ON )

if( NOT LIBORIGIN_STATIC_LIB AND NOT BUILD_SHARED_LIBS )
  message( FATAL_ERROR "You must enable building shared or static library" )
endif()

if( LIBORIGIN_STATIC_LIB )
  # Static library
  add_library( origin_static STATIC $<TARGET_OBJECTS:origin> )
  set_target_properties(origin_static PROPERTIES OUTPUT_NAME "origin" )
endif()

if( BUILD_SHARED_LIBS )
  add_library( origin_shared SHARED $<TARGET_OBJECTS:origin>)
  set_target_properties( origin_shared PROPERTIES OUTPUT_NAME "origin" )
  install( TARGETS origin_shared
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endif()

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}" )
  # Generate a pkg-config file matching this config
  configure_file( "liborigin.pc.in" "liborigin.pc" @ONLY )
  # install Headers
  install(FILES ${devel-headers}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/liborigin
  )
  # install pkg-config file
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/liborigin.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
  )
  if( BUILD_SHARED_LIBS )
    set_target_properties( origin_shared PROPERTIES
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
    )
  endif()
  # command line util
  add_executable(opj2dat opj2dat.cpp)
  if( BUILD_SHARED_LIBS )
    target_link_libraries( opj2dat origin_shared )
  else()
    target_link_libraries( opj2dat origin_static )
  endif()
  install(TARGETS opj2dat
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    OPTIONAL
  )
  # install static lib
  if( LIBORIGIN_STATIC_LIB )
    install( TARGETS origin_static
      LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
      ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    )
  endif()
  # documentation
  install(FILES COPYING FORMAT README
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
    OPTIONAL
  )
  # Doxygen
  find_package( Doxygen )
  if( DOXYGEN_FOUND )
    configure_file( doc/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile )
    add_custom_target( doc
      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    set_property( DIRECTORY APPEND
      PROPERTY
        ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/html
    )
    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
      DESTINATION ${CMAKE_INSTALL_DOCDIR}
      OPTIONAL
    )
  endif()
endif()
