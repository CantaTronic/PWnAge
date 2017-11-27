
cmake_minimum_required(VERSION 2.8)

set(ConfPars
    ConfigParser.h
    ConfigParser.cc
)

# Declare the library
add_library(ConfPars 
# STATIC
SHARED
    ${libfoo_SRCS})

# Specify here the include directories exported by this library
target_include_directories(ConfPars PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(ConfPars PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(ConfPars) 
