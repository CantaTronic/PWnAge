#include "gtest/gtest.h"
#include "TUtils.h"
#include "deviceType.h"
#include <vector>
#include <stdexcept>
#include "omp.h"

using std::vector;

//The basic tests for TUtils.h file

namespace ReadArgs{
  
  int argcF = 3;
  char * argvF1[] = {"prog_name", "4", "CPU"}; 
  char * argvF2[] = {"prog_name", "4", "GPU"}; 
  char * argvF3[] = {"prog_name", "4", "PHI"}; 
  char * argvF4[] = {"prog_name", "4", "any"};
  char * argvF5[] = {"prog_name"}; 
  
 
TEST (testSetDevice, PositiveNos){
    EXPECT_EQ (CPU, setDevice(argcF, argvF1));
    EXPECT_EQ (GPU, setDevice(argcF, argvF2));
    EXPECT_EQ (PHI, setDevice(argcF, argvF3));
    EXPECT_THROW(setDevice(argcF, argvF4),std::domain_error);
    EXPECT_THROW(setDevice(1, argvF5),std::domain_error);
}

TEST (TestSetThrNums, PositiveNos) {
  setThrNums(argcF, argvF1); 
  #pragma omp parallel
  {
  #pragma omp single
      EXPECT_EQ (atoi(argvF1[1]),omp_get_num_threads());
  }
}

}
