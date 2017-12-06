
cmake_minimum_required(VERSION 2.8)

set(MemHandle_SRC
  MemoryHandler.h
  TPtr.h
)

# Declare the library
add_library(MemHandle 
# STATIC
SHARED
    ${MemHandle_SRC})

# Specify here the include directories exported by this library
target_include_directories(MemHandle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}) 

SET_TARGET_PROPERTIES(MemHandle PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(MemHandle Common) 
