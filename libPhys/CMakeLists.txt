
cmake_minimum_required(VERSION 2.8)

set(Phys_SRC
BlattWeisskopf.h
multiindex.h
TEpsilon.h
TEvent.h
TVertex1to2.h
TX.h
VTLorentzVector.h
VTVector3.h
multiindex.cpp
TEpsilon.cpp
TVertex1to2.cpp
TX.cpp
VTLorentzVector.cpp
VTVector3.cpp
)

# Declare the library
add_library(Phys 
# STATIC
SHARED
    ${Phys_SRC})

# Specify here the include directories exported by this library
target_include_directories(Phys PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(Phys PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Phys Common ) 
