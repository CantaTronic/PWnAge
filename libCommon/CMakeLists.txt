
cmake_minimum_required(VERSION 2.8)

set(Common_SRC
    cfloat.h
    TFloat.h
    TGlobals.h
    TReaction.h
    TUtils.h
    OffloadDirectives.h
)

# Declare the library
add_library(Common 
# STATIC
SHARED
    ${Common_SRC})

# Specify here the include directories exported by this library
target_include_directories(Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(Common PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Common) 
