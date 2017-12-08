
cmake_minimum_required(VERSION 2.8)

set(Res_SRC
    TParType.h
    TParArray.h
    TParArray.cpp
    TFitPar.h
    TFitPar.cpp
    TResonanceSet.h
    TResonanceSet.cpp
    TResonance.h
    TResonance.cpp
)

# Declare the library
add_library(Res 
# STATIC
SHARED
    ${Res_SRC})

# Specify here the include directories exported by this library
target_include_directories(Res PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(Res PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Res Common) 
