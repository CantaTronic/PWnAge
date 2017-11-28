file(GLOB SRCS *.cc)

ADD_EXECUTABLE(testutils ${SRCS})

TARGET_LINK_LIBRARIES(testutils
    Common  
    libgtest
    libgmock
)

add_test(NAME testutils
         COMMAND testutils) 
