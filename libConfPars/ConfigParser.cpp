
#include "ConfigParser.h"
#include "TFitPar.h"
#include "TInputFiles.h"
#include "TGlobals.h"
#include "TUtils.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <string>
using std::cerr;
using std::endl;
using std::string;

// enum ParToGet {dataEventFile = 1, mcEventFile};

// void ConfigParser::getByFlag(string flag, string name, ParToGet param) {
//   if(flag == name) {
//     Get(inputFiles->param);
//     continue;
//   }
// }

ConfigParser::ConfigParser(string configFileName, TResonanceSet * set, TInputFiles * inputFiles) {
//   ParToGet par = dataEventFile;
  TResonance res;
  std::ifstream ifile;
  checkOpen(configFileName, ifile);
  while(1) {
    string line;
    bool not_eof = std::getline(ifile, line); // get next line
    if(!not_eof) { // end of file
      break; // exit
    }
    if(line == "" || line[0] == '#' || line[0] == ';')
      continue; // empty string or comment
    // re-initialize input string stream
    iss.clear();
    iss.str(line);
    iss.seekg(0);
//     std::cout<<dataEventFile<<std::endl;
    // begin parsing line
    string flag, delim;
    iss >> flag >> delim;
    if(flag == "" || delim != "=")
      continue; // not parameter string
    if(flag == "dataEventFile") {
      Get(inputFiles->dataEventFile);
      continue;
    }
    if(flag == "dataCacheFile") {
      Get(inputFiles->dataCacheFile);
      continue;
    }
    if(flag == "mcEventFile") {
      Get(inputFiles->mcEventFile);
      continue;
    }
    if(flag == "mcCacheFile") {
      Get(inputFiles->mcCacheFile);
      continue;
    }
    if(flag == "resonance") { // new resonance begins
      res = set->Add(Get<string>()); // put resonance in set
      continue;
    }
    if(!res) continue;
    // read resonance parameter only if any resonances added into set
    if(flag == "M") {
      GetFitPar(res._M(), 1.0/*,  0.1, 3.0*/);
    } else if(flag == "G") {
      GetFitPar(res._G(), 0.2/*, 0.01, 1.0*/);
    } else if(flag == "r") {
      GetFitPar(res._r(), 0.73/*, 0.200, 1.0*/);
    } else if(flag == "coupling") {
      GetFitPar(res._Coupling(), 1/*, -100, 100*/);
    } else if(flag == "phase") {
      // phases in config file are given in degrees
      // should be converted to radians
      GetProdPhase(res._Phase(), 0/*, -360, 360*/);
    } else if(flag == "J") {
      res.J(Get<unsigned>());
    } else if(flag == "P") {
      res.P(Get<unsigned>());
    } else if(flag == "C") {
      res.C(Get<unsigned>());
    } else if(flag == "type") {
      res.type(Get<int>());
    } else if(flag == "symmetryMultiplier") {
      GetSymmetryMultiplier(res);
    }
  }
  ifile.close();
}

template<class T>
void ConfigParser::Get(T & v) {
  iss>>v;
}
template<class T>
T ConfigParser::Get() {
  T v;
  Get(v);
  return v;
}


void ConfigParser::GetFitPar(TFitPar p, double _v) {
  string type = "free";
  iss >> _v >> type;
  type.resize(3, '0');
  if(type == "fix") {
    p.Set(_v, 0);
  } else if(type == "lim") {
    double _vmin = 1e-6, _vmax = 1e6;
    iss >> _vmin >> _vmax;
    p.Set(_v, _vmin, _vmax);
  } else {
    p.Set(_v);
  }
}

void ConfigParser::GetProdPhase(TFitPar p, double _v) {
  string type = "free";
  iss >> _v >> type;
  type.resize(3, '0');
  if(type == "fix") {
    p.Set(_v, 0);
    return;
  }
  double _vmin = -360, _vmax = 360;
  if(type == "lim") {
    iss >> _vmin >> _vmax;
  }
  p.Set(_v*_pi/180., _vmin*_pi/180., _vmax*_pi/180.);
}

void ConfigParser::GetSymmetryMultiplier(TResonance & res) {
  for(unsigned i = 0; i < 3; i++) {
    TFloat m = 0;
    iss>>m;
    res.SymmetryMultiplier(i) = m;
  }
}

