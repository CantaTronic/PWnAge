
#pragma once

#include <iostream>
#include <stdlib.h>
#include <omp.h>
#include <string>
#include <stdexcept>
#include <exception>
#include "deviceType.h"
#include <cstdlib> 
#include <fstream>

using std::cout;
using std::endl;

/*
Usefull utility functions for make main.cpp shorter and nicier
*/

void checkOpen(std::string configFileName, std::ifstream& ifl)  {
  ifl.open(configFileName.c_str());
  if (!ifl) {
    std::cerr << "Unable open file for reading. Exiting..." << std::endl;
    exit(-1);
  }
}

void setThrNums(int argc, char * argv[]){
  //set up number of threads. tested
    unsigned thrNum = 1;	//init thrNum
    if(argc > 1 && argv[1]>0) 
            thrNum = atoi(argv[1]);
    omp_set_num_threads(thrNum);  //set number of threads
}

void devPrint(std::string devName)  {
  /*just cute looking control print, nothing more*/
  cout<<"Starting "<<devName<<" calculations..."<<endl;
}

deviceType setDevice(int argc, char * argv[]){
  /*
   * The function which controlls the proper initial values for correct work of a program. 
   */
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
          return PHI;
        } else {
          throw std::domain_error ("Undefined device! Exiting...");
        }
    } else {
        throw std::domain_error ("No device provided! Exiting...");
    }
}

deviceType setArguments(int argc, char * argv[]) {
  /*
   The main function here. Parse comand line input and set up the device for calculations and number of threads (for CPU-like architectures)
   */
  setThrNums(argc, argv); 
  try {
    deviceType dev = setDevice(argc, argv);
    return dev;
  } catch (std::exception const& exc) {
    std::cerr<<exc.what()<<endl;
    exit(0);
  }
}
