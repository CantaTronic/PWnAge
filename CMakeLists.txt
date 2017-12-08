# Folder structure (to be updated):
#
# |
# +--CMakeList.txt (with C++11 content)
# +--libConfPars
# |  |
# |  +--
# +--libCommon
# +--libRes
# +--libPhys
# +--libCache
# +--libMemHandle
# +--Other
# +--test

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.6)

# setting up the project's name
project(PWnAge)
# Still don't set the output folder where our program will be created - maybe, later?

# #compilation/linking keys
add_definitions(-Wall -O2 -fPIC -std=c++11 -g -Wno-write-strings)
# 
# #CMAKE_BINARY_DIR is the top-level directory of the building tree
#CMAKE_SOURCE_DIR is dir from which we will start to make project
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
# 
#the folder for binary executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#looking for omp package...
FIND_PACKAGE( OpenMP REQUIRED)

if(OPENMP_FOUND)
  message("OPENMP FOUND")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif() 


# enable_testing()
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

#folder there we will be look for the libraries
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})
# 
# The following folder will be included
include_directories(libConfPars)
include_directories(libCommon)
include_directories(libRes)
include_directories(libPhys)
include_directories(libCache)
include_directories(libMemHandle)
include_directories(Other)

add_subdirectory(libConfPars)
add_subdirectory(libCommon)
add_subdirectory(libRes)
add_subdirectory(libPhys)
add_subdirectory(libCache)
add_subdirectory(libMemHandle)
add_subdirectory(Other)
add_subdirectory(test)
# 
# #compile test_JpsiToKKpi0.cc into a target "PWnAge" 
add_executable(${PROJECT_NAME} main/test_JpsiToKKpi0.cpp)
# 
target_link_libraries(${PROJECT_NAME} ConfPars Common Res Phys Cache MemHandle Other)
# 
# #install our binaries (have no idea, why do we need this - they are still installed automatically without it)
# #DECTINATION and TARGETS are options of install() command
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
