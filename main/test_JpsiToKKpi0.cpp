
#include "TJpsiToKKpi0Const.h"  /*this is a header of the concrete reaction we want to make calculations for. Reactions are being inherited from TReaction abstract class*/
#include "TPWAFunction.h"
#include "TCache.h"
// #include "TStat.h"
#include <omp.h>
#include <TUtils.h>
#include "deviceType.h"   //currently supported device types are CPU, GPU, PHI
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;
#include <iomanip>
using std::setw;

#include <typeinfo>
#define quote(x) #x

/*
 To be runned with a device and number of threads given. 
 Example: ./code CPU 4
 */

int main(int argc, char * argv[]) {
  //model initialization
  gReaction = new TJpsiToKKpi0Const;
  
  //suppress verbose control print both about function pecularities and caching
  TPWAFunction::verbosity = 3; //TODO: организовать какой-то один переключатель для включения/выключения контрольной печати 
  TCache::verbosity = 3;   //TODO: спрятать эту хрень из пользовательского интерфейса вместе с заголовком TCache.h

  deviceType dev = setArguments(argc, argv);

  //to the factory method:
  TPWAFunction * fcn;
  fcn = TPWAFunction::Create(dev,"resonances.ini");
  

  
  TFloat time_call;
  double par[100];
  cerr<<" ==== FUMILI ===="<<endl;
  fcn->GetParameters(par);/*
  time_call = fcn.Fit(ROOTMinimizer::FUMILI);
  cerr<<setw(3)<<"12"<<' '<<time_call<<endl;*/
  
//   cerr<<" ==== MIGRAD (USER GRADIENTS) ===="<<endl;
//   fcn.SetParameters(par);
//   time_call = fcn.Fit(ROOTMinimizer::MIGRAD_G);
//   cerr<<setw(3)<<"12"<<' '<<time_call<<endl;
//   cerr<<" ==== MIGRAD (MINUIT GRADIENTS) ===="<<endl;
//   fcn.SetParameters(par);
//   time_call = fcn.Fit(ROOTMinimizer::MIGRAD);
//   cerr<<setw(3)<<"12"<<' '<<time_call<<endl;
//   cerr<<" ==== SIMPLEX ===="<<endl;
//   fcn.SetParameters(par);
//   time_call = fcn.Fit(ROOTMinimizer::SIMPLEX);
//   cerr<<setw(3)<<"12"<<' '<<time_call<<endl;

/*
  // FCN call time tests
  cerr<<"thr time"<<endl;
  // ==== PHI ====
  unsigned n_thr_phi[] = {1, 2, 4, 6, 8, 12, 16, 20, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224};
  #pragma offload target(mic:0) nocopy(gReaction)
  {
    gReaction = new TJpsiToKKpi0Const;
  }
  for(unsigned i = 0; i < sizeof(n_thr_phi)/sizeof(unsigned); i++) {
    unsigned t_n_thr_phi = n_thr_phi[i];
    #pragma offload target(mic:0) in(t_n_thr_phi) nocopy(gReaction)
    {
      omp_set_num_threads(t_n_thr_phi);
    }
    TPWAFunctionPHI fcn("resonances.ini");
    fcn.TestGrad(10, t_n_thr_phi);
  }
  // ==== GPU ====
  {
    TPWAFunctionGPU fcn("resonances.ini");
    fcn.TestGrad(10, 1);
  }
  // ==== CPU ====
  unsigned n_thr_cpu[] = {1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16, 20, 24, 28, 32, 36, 40, 44, 48};
  for(unsigned i = 0; i < sizeof(n_thr_cpu)/sizeof(unsigned); i++) {
    omp_set_num_threads(n_thr_cpu[i]);
    TPWAFunctionCPU fcn("resonances.ini");
    fcn.TestGrad(10, n_thr_cpu[i]);
  }
*/
  free (fcn);
  return 0;
}
