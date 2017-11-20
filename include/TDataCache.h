
#pragma once

#include "TCache.h"
#include <string>
using std::string;

class TEvent;

class TDataCache: public TCache {
public:
  TDataCache(TResonanceSet * r, string eventFileName, string cacheFileName = "");
  virtual ~TDataCache();
  template<class randomAccessIterator>
  void CalcAmplitudeParts(randomAccessIterator begin, randomAccessIterator end);
  virtual void CalcAmplitudeParts(TEvent * begin, TEvent * end);
protected:
  virtual void ReadEventFile(string eventFileName, string cacheFileName = "");
  virtual bool LoadCache(string filename);
  virtual void SaveCache(string filename);
  virtual void Allocate();
private:
  // TODO copy-constructor & copy-assignment
  TDataCache(const TDataCache & x);              // copy-constructor
  TDataCache & operator= (const TDataCache & x); // copy-assignment
};

template<class randomAccessIterator>
inline void TDataCache::CalcAmplitudeParts(randomAccessIterator begin, randomAccessIterator end) {
  CalcAmplitudeParts(&(*begin), &(*end));
}
