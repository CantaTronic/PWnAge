
#pragma once

#include <iostream>
#include <stdlib.h>

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
//     argCtrl(argc, argv);  //check out the arguments given
    unsigned thrNum = 1;	//init thrNum
    if(argc > 1 && argv[1]>0) 
            thrNum = atoi(argv[1]);
    omp_set_num_threads(thrNum);  //set number of threads
    testThrNumbs(); //checkout whether theads were set up
}
