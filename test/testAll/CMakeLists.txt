

include_directories("${PROJECT_SOURCE_DIR}/include")

link_directories("${PROJECT_SOURCE_DIR}/include")

message ("${PROJECT_SOURCE_DIR}/include")

file(GLOB SRCS *.cc)

# ADD_EXECUTABLE(testutils testTUtils.cc)
# ADD_EXECUTABLE(testPWAFunc testTPWAFunction.cc)

ADD_EXECUTABLE(testAll ${SRCS})

TARGET_LINK_LIBRARIES(testAll
    Common  
    Other
    libgtest
    libgmock
)

add_test(NAME testAll
         COMMAND testAll) 

# add_test(NAME testPWAFunc
#          COMMAND testpwa) 
