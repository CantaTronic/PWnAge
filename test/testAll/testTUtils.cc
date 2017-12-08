#include "gtest/gtest.h"
#include "TUtils.h"
#include "deviceType.h"
#include <vector>
#include <stdexcept>
#include "omp.h"
#include <fstream>

using std::vector;
using  std::ifstream;

//The basic tests for TUtils.h file

namespace ReadArgs{
  
  int argcF = 3;
  char PName[] = "prog_name";
  char * argvF1[] = {PName, "4", "CPU"}; 
  char * argvF2[] = {PName, "4", "GPU"}; 
  char * argvF3[] = {PName, "4", "PHI"}; 
  char * argvF4[] = {PName, "4", "any"};
  char * argvF5[] = {PName}; 
  
 
TEST (testTUtils, testSetDevice){
    EXPECT_EQ (CPU, setDevice(argcF, argvF1));
    EXPECT_EQ (GPU, setDevice(argcF, argvF2));
    EXPECT_EQ (PHI, setDevice(argcF, argvF3));
    EXPECT_THROW(setDevice(argcF, argvF4),std::domain_error);
    EXPECT_THROW(setDevice(1, argvF5),std::domain_error);
}

TEST (testTUtils, TestSetThrNums) {
  setThrNums(argcF, argvF1); 
  #pragma omp parallel
  {
  #pragma omp single
      EXPECT_EQ (atoi(argvF1[1]),omp_get_num_threads());
  }
}

}

namespace ReadFiles{
  
  ifstream ifl;
  std::string configFileNameF = "config.ini";
//   std::string configFileNameT = "/nfs/store2.jinr.ru/user/v/vtokareva/parallel_pwa/PWnAge/resonances.ini";
  
TEST (testTUtils, TestCheckOpen) {
  EXPECT_EXIT (checkOpen(configFileNameF,ifl),::testing::ExitedWithCode(255),  "Unable open file for reading. Exiting...");
//   EXPECT_DEATH_IF_SUPPORTED (checkOpen(configFileNameT,ifl),/*::testing::ExitedWithCode(255), */ "Unable open file for reading. Exiting...");
}

}


