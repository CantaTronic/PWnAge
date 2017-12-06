#include "gtest/gtest.h"
#include "ConfigParser.h"

class testConfigParser : public ::testing::Test {
public:
testConfigParser(){
    ConfigParser prs;
}

virtual ~testConfigParser();

// before each test).
virtual void SetUp();

// immediately after each test
virtual void TearDown();

};

TEST (testConfPars, ElementaryTest) {
  ConfigParser my_test;
  EXPECT_EQ (my_test.test("ConfPars"),"test ConfPars");
}

