
#define PWA_DISABLE_OFFLOAD
#include "TEvent.h"
#include "TDataCache.h"
#include "TResonanceSet.h"
#include "TCacheFile.h"
#include "TEpsilon.h"
#include "TVertex1to2.h"
#include "multiindex.h"
#include "BlattWeisskopf.h"
#include <algorithm>
using std::copy;
#include <fstream>
using std::cerr;
using std::endl;
#include <cstring>
#include <omp.h>
#include "TReaction.h"

TDataCache::TDataCache(TResonanceSet * r, string eventFileName, string cacheFileName)
: TCache(r) {
  unsigned n_thr = omp_get_max_threads();
  if(verbosity >= 1)
    cerr<<"Doing TDataCache with "<<n_thr<<" OpenMP threads"<<endl;
  if(!LoadCache(cacheFileName)) { // пробуем подгрузку из кэшей
    // если не нашли кеша -- пересчитываем амплитуды заново
    ReadEventFile(eventFileName, cacheFileName);
  }
  if(verbosity >= 1)
    cerr<<"Loaded total "<<nev<<" events"<<endl;
}

TDataCache::~TDataCache() {
  Cleanup();
}

void TDataCache::ReadEventFile(string eventFileName, string cacheFileName) {
  vector<TEvent> events;
  TEvent tmp;
  if(verbosity >= 2)
    cerr<<"Reading file "<<eventFileName<<endl;
  std::ifstream ifs(eventFileName.c_str());
  if(!ifs || !ifs.is_open()) {
    cerr<<"Error opening file, exiting..."<<endl;
    exit(1);
  }
  while(1) {
    TFloat dum;
    ifs>>tmp.number>>tmp.weight>>dum>>dum;
    TFloat p0x, p0y, p0z, p0t;
    TFloat p1x, p1y, p1z, p1t;
    TFloat p2x, p2y, p2z, p2t;
    ifs>>p0x>>p0y>>p0z>>p0t;
    ifs>>p1x>>p1y>>p1z>>p1t;
    ifs>>p2x>>p2y>>p2z>>p2t;
    if(ifs.eof()) break; // check header
    VTLorentzVector & p0 = tmp.particles[0];
    VTLorentzVector & p1 = tmp.particles[1];
    VTLorentzVector & p2 = tmp.particles[2];
    p0.SetPxPyPzE(p0x, p0y, p0z, p0t);
    p1.SetPxPyPzE(p1x, p1y, p1z, p1t);
    p2.SetPxPyPzE(p2x, p2y, p2z, p2t);
    // boost to cms
    VTLorentzVector ptot = p0+p1+p2;
    VTVector3 boost = 1./ptot.E()*ptot.Vect();
    p0.Boost(-boost);
    p1.Boost(-boost);
    p2.Boost(-boost);
    events.push_back(tmp);
  }
  ifs.close();
  nev = events.size();
  if(nev == 0) {
    cerr<<"No data found in file. Exiting..."<<endl;
    exit(1);
  }
  if(verbosity >= 2)
    cerr<<"Read file "<<eventFileName<<endl;
  CalcAmplitudeParts(events.begin(), events.end());
  events.clear();
  if(cacheFileName != "") SaveCache(cacheFileName);
}
// TODO custom particle masses other than Jpsi -> K+ K- pi0
void TDataCache::CalcAmplitudeParts(TEvent * begin, TEvent * end) {
  if(verbosity >= 1)
    fprintf(stderr, "Calculating amplitudes & filling caches for %li events\n", end-begin);
  double start = omp_get_wtime();
  nev = end-begin;
  Allocate();
  cfloat * complex = new cfloat[resonances->L()*12]; // [l][kchan][mu]
  for(TEvent * it = begin; it != end; it++) {
    int i = it-begin;
    if(verbosity >= 1 && i % 50000 == 0)
      fprintf(stderr, "Filling block for event %9i\n", i);
    weight[i] = it->weight;
    // Using event calculates parts of the amplitude, stores them to corresponding arrays
    VTLorentzVector & ppi0 = it->particles[0];
    VTLorentzVector & pKp  = it->particles[1];
    VTLorentzVector & pKm  = it->particles[2];
    VTLorentzVector P = ppi0+pKp+pKm;

    s_kchan[0][i] = (pKp+ppi0).Mag2();
    s_kchan[1][i] = (pKm+ppi0).Mag2();
    s_kchan[2][i] = (pKm+pKp).Mag2();

    memset(complex, 0, sizeof(cfloat)*resonances->L()*12); // [l][kchan][mu]
    for(int kchan=0; kchan<3; kchan++) {  // loop over kinematic channel
      TVertex1to2 * v0 = 0;
      TVertex1to2 * v1 = 0;
      if(kchan == 0) { // R = K+ pi0, K- spec
        v0 = new TVertex1to2(resonances->L(), (pKp+ppi0), pKm);
        v1 = new TVertex1to2(resonances->L(), pKp, ppi0);
      }
      if(kchan == 1) { // K-pi0
        v0 = new TVertex1to2(resonances->L(), (pKm+ppi0), pKp);
        v1 = new TVertex1to2(resonances->L(), pKm, ppi0);
      }
      if(kchan == 2) { //  K+K-
        v0 = new TVertex1to2(resonances->L(), (pKm+pKp), ppi0);
        v1 = new TVertex1to2(resonances->L(), pKm, pKp);
      }
      VTLorentzVector v0kT (v0->X[1]->get0123(1, 0, 0, 0),
                            v0->X[1]->get0123(0, 1, 0, 0),
                            v0->X[1]->get0123(0, 0, 1, 0),
                            v0->X[1]->get0123(0, 0, 0, 1));
      TFloat v0kT2 = v0kT.Mag2();
      VTLorentzVector v1kT (v1->X[1]->get0123(1, 0, 0, 0),
                            v1->X[1]->get0123(0, 1, 0, 0),
                            v1->X[1]->get0123(0, 0, 1, 0),
                            v1->X[1]->get0123(0, 0, 0, 1));
      v1kT2[kchan][i] = v1kT.Mag2();

      for(unsigned l = 0; l < resonances->L(); l++) {
        // construct amplParts from X operators
        // use CMS
        for(int mu=0; mu<4; mu++)
          complex[l*12+kchan*4+mu] = 0.;
        for(int iperm = 0; iperm < gEpsilon.permutation012Number; iperm++) {
          int mu, nu, lam, sig;
          TFloat eparity;
          gEpsilon.getPermutation012(iperm, eparity, mu, nu, lam, sig);
          for(long int mi=0; mi<pown(4,l); mi++) {
            int * indv0 = new int[l+1]; indv0[0] = nu;
            int * indv1 = new int[l+1]; indv1[0] = lam;
            multiindex(mi, l, indv0+1);
            copy(indv0+1, indv0+1+l, indv1+1); //std::copy
            complex[l*12+kchan*4+mu] += eparity * P[sig] *
                v0->X[l+1]->get(indv0) * v1->X[l+1]->get(indv1) * metrics_product(l, indv0+1) ;
            delete [] indv0;
            delete [] indv1;
          }
        }
        TFloat blw_prod = sqrt( BlattWeisskopfFact(l+1, -v0kT2, gReaction->rRes()) );
        for(int mu=0; mu<4; mu++)
          complex[l*12+kchan*4+mu] /= blw_prod;
      }
      delete v0;
      delete v1;
    }
    for(unsigned int r = 0; r < resonances->N(); r++) {
      int l = resonances->Res(r).J() - 1;
      for(int k = 0; k < 3; k++) {
        for(int m = 0; m < 4; m++) {
          if(!gReaction->Dens(m)) continue;
          precalc_factor[l*3*4+k*4+m][i] = complex[l*3*4+k*4+m];
        }
      }
    }
  }
  delete [] complex;
  if(verbosity >= 1)
    cerr<<"Time of TDataCache::calcAmplitudeParts calculation "<<omp_get_wtime()-start<<endl;
}

void TDataCache::Allocate() {
  Cleanup();
  mem_alloc += v1kT2.Allocate(nev);
  mem_alloc += s_kchan.Allocate(nev);
  mem_alloc += weight.Allocate(nev);
  for(unsigned l = 0; l < resonances->L(); l++) {
    for(unsigned k = 0; k < 3; k++) {
      for(unsigned m = 0; m < 4; m++) {
        if(!gReaction->Dens(m)) continue;
        mem_alloc += precalc_factor[l*3*4+k*4+m].Allocate(nev);
      }
    }
  }
  if(verbosity >= 3)
    cerr<<"Allocated memory"<<endl;
}

void TDataCache::SaveCache(string filename) {
  if(verbosity >= 2)
    cerr<<"Saving cache file "<<filename<<endl;
  OCacheFile cf(filename);
  cf.Write(resonances);
  cf.Write(nev);
  cf.Write(v1kT2, nev);
  cf.Write(s_kchan, nev);
  cf.Write(weight, nev);
  cf.Write(precalc_factor, nev);
  if(verbosity >= 2)
    cerr<<"Saved cache file"<<endl;
}

bool TDataCache::LoadCache(string filename) {
  if(verbosity >= 2)
    cerr<<"Loading cache file "<<filename<<endl;
  ICacheFile cf(filename);
  if(!cf) {
    if(verbosity >= 2)
      cerr<<"No cache exists"<<endl;
    return false;
  }
  TResonanceSet tmp;
  cf.Read(&tmp);
  if(tmp != *resonances) {
    if(verbosity >= 2)
      cerr<<"Outdated cache exists (resonances changed)"<<endl;
    return false;
  }
  if(verbosity >= 2)
    cerr<<"Up-to-date cache exists"<<endl;
  cf.Read(nev);
  Allocate();
  cf.Read(v1kT2, nev);
  cf.Read(s_kchan, nev);
  cf.Read(weight, nev);
  cf.Read(precalc_factor, nev);
  if(verbosity >= 2)
    cerr<<"Loaded cache file"<<endl;
  return true;
}
