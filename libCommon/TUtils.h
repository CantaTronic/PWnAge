
#pragma once

#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <string>
// #include <cstring>
#include <stdexcept>
#include <exception>
#include "deviceType.h"
#include <cstdlib> 

using std::cout;
using std::endl;

/*
Usefull utility functions for make main.cpp shorter and nicier
*/

void testThrNumbs(){
  /*prints number of threads in parallel region*/
  #pragma omp parallel
  {
    #pragma omp single
    cout<<"Set up "<<omp_get_num_threads()<<" threads"<<endl;
  }
  
}

void argCtrl(int argc, char * argv[]){
  //control print of an arguments
  cout<<"Arguments are given:"<<endl;
  for (int i = 0; i<argc; i++)
          cout<<"\t"<<i<<"\t"<<argv[i]<<endl; 
  
}

void setThrNums(int argc, char * argv[]){
    unsigned thrNum = 1;	//init thrNum
    if(argc > 1 && argv[1]>0) 
            thrNum = atoi(argv[1]);
    omp_set_num_threads(thrNum);  //set number of threads
    testThrNumbs(); //checkout whether theads were set up
}

void devPrint(std::string devName)  {
  cout<<"Starting "<<devName<<" calculations..."<<endl;
}

deviceType setDevice(int argc, char * argv[]){
    if (argc > 2){
        std::string dev = std::string(argv[2]);
        if (dev == "CPU") {
          devPrint(dev);
          return CPU;
        } else if (dev == "GPU") {
          devPrint(dev);
          return GPU;
        } else if (dev == "PHI") {
          devPrint(dev);
          return CPU;
        } else {
          throw std::domain_error ("Undefined device! Exiting...");
        }
    } else {
        throw std::domain_error ("No device provided! Exiting...");
    }
}

int returnNum (int num) {
  //simplest function to test tests
  return num;
}

deviceType setArguments(int argc, char * argv[]) {
  setThrNums(argc, argv); 
  try {
    deviceType dev = setDevice(argc, argv);
    return dev;
  } catch (std::exception const& exc) {
    std::cerr<<exc.what()<<endl;
    exit(0);
  }
}
