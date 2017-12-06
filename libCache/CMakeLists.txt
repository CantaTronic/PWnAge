
cmake_minimum_required(VERSION 2.8)

set(Cache_SRC
TCache.h
TCalcCache.h
TDataCache.h
TCacheFile.h
TCache.cpp
TCalcCache.cpp
TDataCache.cpp
TCacheFile.cpp
)

# Declare the library
add_library(Cache 
# STATIC
SHARED
    ${Cache_SRC})

# Specify here the include directories exported by this library
target_include_directories(Cache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(Cache PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Cache Phys Common Res ) 
