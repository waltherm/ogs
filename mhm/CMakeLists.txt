cmake_minimum_required(VERSION 2.8.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
project(mhm)
enable_language (Fortran)
add_definitions(-DGFORTRAN)


list(APPEND CMAKE_PREFIX_PATH
	$ENV{HOMEBREW_ROOT}             # Homebrew package manager on Mac OS
	$ENV{CMAKE_LIBRARY_SEARCH_PATH} # Environment variable, Windows
	${CMAKE_LIBRARY_SEARCH_PATH})

set (NETCDF_F90 "YES")
find_package(NetCDF REQUIRED)
include_directories(${NETCDF_INCLUDES})

include_directories(${CMAKE_CURRENT_BINARY_DIR}/lib)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(lib)
add_subdirectory(src)



set(SOURCES
	mHM_MainCaller.f90
)

add_library(mhm-com ${SOURCES})
add_dependencies(mhm-com mhm)

# Enable C preprocessor on all files to enable #ifdefs
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS -cpp)

SET(mhm_com mhm-com mhm libmhm mhm-base CACHE STRING "mhm_com" FORCE) 
