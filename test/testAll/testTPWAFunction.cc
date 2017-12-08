#include "gtest/gtest.h"
#include "TPWAFunction.h"
#include "TPWAFunctionCPU.h"
#include "deviceType.h"
#include <typeinfo>
#include <iostream>

#define quote(x) #x

class testTPWAFunction : public ::testing::Test {
public:
// before each test).
virtual void SetUp(); /*{
  TPWAFunction * fcn;
}*/

// immediately after each test
virtual void TearDown();

};

TEST(testTPWAFunction, Create) {
  TPWAFunction * fcn;
  fcn = TPWAFunction::Create(deviceType::CPU,"resonances.ini");
// //     std::cout<<typeid(fcn).name()<<"\t"/*<< quote(fcn)*/ <<std::endl;
//    EXPECT_EQ (typeid(fcn).name(), typeid(TPWAFunctionCPU).name());

}

