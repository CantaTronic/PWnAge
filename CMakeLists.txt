# Folder structure:
#
# |
# +--CMakeList.txt (with C++11 content)
# +--include/
# |  |
# |  +--*.h
# |
# +--src/
# |  |
# |  +--test_JpsiToKKpi0.cpp
# |  +--*.cc
# -

# Specify the minimum version for CMake
cmake_minimum_required(VERSION 2.8)

# setting up the roject's name as hello
project(PWnAge)
# Set the output folder where your program will be created


#compilation/linking keys
add_definitions(-Wall -O2 -fPIC -std=c++0x -g -fopenmp)

#CMAKE_BINARY_DIR is the top-level directory of the building tree
#CMAKE_SOURCE_DIR is dir from which we will start to make project
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)

#the folder for binary executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})


# enable_testing()
# find_package(GTest REQUIRED)
# include_directories(${GTEST_INCLUDE_DIRS})

#folder there we will be look for the libraries
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}/include")

# looking for the sources in src/ dir
file(GLOB SOURCES src/*.cc)

#compile test_JpsiToKKpi0.cc into a target "PWnAge" 
add_executable(${PROJECT_NAME} "src/test_JpsiToKKpi0.cpp" 
src/TJpsiToKKpi0Const.cpp 
src/TCache.cpp
)#${SOURCES})

# target_link_libraries(${PROJECT_NAME} ${GTEST_BOTH_LIBRARIES})

#install our binaries (have no idea, why do we need this - they are still installed automatically without it)
#DECTINATION and TARGETS are options of install() command
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
