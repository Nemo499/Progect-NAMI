cmake_minimum_required(VERSION 3.5)
project(map_linker)

### Compile options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#PUGI_XML
find_path(PUGI_XML_INCLUDE_DIRS pugixml.hpp
	  PATHS /usr/include
         /usr/local/include)

if(PUGI_XML_INCLUDE_DIRS)
   find_library (PUGI_XML_LIBRARIES
                NAMES pugixml libpugixml
                PATHS /usr/lib
                      /usr/local/lib)
   if(PUGI_XML_LIBRARIES)
      set(PUGI_XML_FOUND true)
   endif(PUGI_XML_LIBRARIES)
endif(PUGI_XML_INCLUDE_DIRS)

## GeographicLib
find_package(PkgConfig)
find_path(GeographicLib_INCLUDE_DIR GeographicLib/Config.h
  PATH_SUFFIXES GeographicLib
)

set(GeographicLib_INCLUDE_DIRS ${GeographicLib_INCLUDE_DIR})
find_library(GeographicLib_LIBRARIES
  NAMES Geographic
)

set(HPP_DIRECTORY PUGI_XML_INCLUDE_DIRS GeographicLib_INCLUDE_DIRS ./header)
include_directories(${HPP_DIRECTORY})

set(SOURCES main.cpp)

add_executable(map_linker ${SOURCES})

target_link_libraries(map_linker
  Geographic
  pugixml
)
