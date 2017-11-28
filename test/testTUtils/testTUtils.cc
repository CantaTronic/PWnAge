#include "gtest/gtest.h"
#include "TUtils.h"

//There will be basic tests for my Hello_world program 
 
TEST (ReurnTest, PositiveNos) { 
    EXPECT_EQ (3, returnNum(3));
    EXPECT_EQ (-10, returnNum(-10));
    EXPECT_EQ (-10, 5*returnNum(-2));
    EXPECT_EQ (-10, -returnNum(5)*returnNum(2));
}
 

