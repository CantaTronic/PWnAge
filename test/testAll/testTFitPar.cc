
#include "TFitPar.h" 
#include "TParArray.h"
#include "gtest/gtest.h"
#include <iostream>

using std::cout;
using std::endl;

TEST (testTFitPar, Offset) {
  TParArray * par;
  TFitPar testPar(par, 5);
  EXPECT_EQ (5,testPar.getOffset());
//   cout<<testPar.Name()<<endl;;
}
