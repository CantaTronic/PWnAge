
cmake_minimum_required(VERSION 2.8)

set(ConfPars_SRC
    ConfigParser.h
    ConfigParser.cpp
    TInputFiles.h
)

# include_directories(libRes)
# link_directories(../libRes)

# Declare the library
add_library(ConfPars 
# STATIC
SHARED
    ${ConfPars_SRC})

# Specify here the include directories exported by this library
target_include_directories(ConfPars PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(ConfPars PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(ConfPars Common Res) 
