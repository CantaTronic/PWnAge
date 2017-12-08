#include "gtest/gtest.h"
#include "ConfigParser.h"

class testConfigParser : public ::testing::Test {
  
testConfigParser(){
    ConfigParser prs;
}

virtual ~testConfigParser();

// before each test).
virtual void SetUp();

// immediately after each test
virtual void TearDown();

};

TEST (testConfPars, PositiveNos) {
  testConfigParser my_test;
  ACCERT_EQ(my_test.test("ConfPars"),"test ConfPars")
}

