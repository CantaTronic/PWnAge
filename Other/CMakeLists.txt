
cmake_minimum_required(VERSION 2.8)

set(Other_SRC
TPWAFunction.h
TPWAFunctionCPU.h
TJpsiToKKpi0Const.h
TCalcCPU.h
ObjectiveFunction.h
# Minimizer.h

TPWAFunction.cpp
TPWAFunctionCPU.cpp
TJpsiToKKpi0Const.cpp
TCalcCPU.cpp
# Minimizer.cpp
)

# Declare the library
add_library(Other 
# STATIC
SHARED
    ${Other_SRC})

# Specify here the include directories exported by this library
target_include_directories(Other PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(Other PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Other Cache Phys Common Res ConfPars) 
