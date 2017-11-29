#include "gtest/gtest.h"
#include "TUtils.h"
#include "deviceType.h"

//There will be basic tests for my Hello_world program 
 
TEST (ReurnTest, PositiveNos) { 
    EXPECT_EQ (3, returnNum(3));
    EXPECT_EQ (-10, returnNum(-10));
    EXPECT_EQ (-10, 5*returnNum(-2));
    EXPECT_EQ (-10, -returnNum(5)*returnNum(2));
}
 
TEST (SetDevice, PositiveNos){
    int argcF = 3;
    char * argvF[] = {"prog_name", "4", "CPU"};
    EXPECT_EQ (CPU, setDevice(argcF, argvF));
}

