
#pragma once

#include "OffloadDirectives.h"
#include "TPtr.h"

PWA_START_OFFLOAD
#include <iostream>
#include <string>
PWA_END_OFFLOAD
using std::string;

class TResonanceSet;

PWA_START_OFFLOAD
class ICacheFile {
public:
  enum Type {kFile, kBuffer};
  ICacheFile(string * buf);
  ICacheFile(string name);
  ~ICacheFile();
  void Read(string & s);
  template<class T>
  void Read(T & t);
  template<class T>
  void Read(vector<T> & v);
  template<class T>
  void Read(TPtr<T> & t, unsigned n);
  template<class T>
  void Read(TPtrVec<T> & v, unsigned n);
  void Read(TResonanceSet * set);
  operator bool();
private:
  std::istream * is;
  Type type;
  string * buffer;
};
PWA_END_OFFLOAD

class OCacheFile {
public:
  enum Type {kFile, kBuffer};
  OCacheFile(string * buf);
  OCacheFile(string name);
  ~OCacheFile();
  void Write(string & s);
  template<class T>
  void Write(T & t);
  template<class T>
  void Write(vector<T> & v);
  template<class T>
  void Write(TPtr<T> & t, unsigned n);
  template<class T>
  void Write(TPtrVec<T> & v, unsigned n);
  void Write(TResonanceSet * set);
  operator bool();
private:
  std::ostream * os;
  Type type;
  string * buffer;
};

template<class T>
void OCacheFile::Write(T & t) {
  os->write(reinterpret_cast<char*>(&t), sizeof(T));
}

template<class T>
void OCacheFile::Write(vector<T> & v) {
  os->write(reinterpret_cast<char*>(v.data()), sizeof(T)*v.size());
}

template<class T>
void OCacheFile::Write(TPtr<T> & t, unsigned n) {
  if(!t) return;
  os->write(reinterpret_cast<char*>(t.p), sizeof(T)*n);
}

template<class T>
void OCacheFile::Write(TPtrVec<T> & v, unsigned n) {
  for(unsigned i = 0; i < v.size(); i++)
    Write(v[i], n);
}

template<class T>
void ICacheFile::Read(T & t) {
  is->read(reinterpret_cast<char*>(&t), sizeof(T));
}

template<class T>
void ICacheFile::Read(vector<T> & v) {
  is->read(reinterpret_cast<char*>(v.data()), sizeof(T)*v.size());
}

template<class T>
void ICacheFile::Read(TPtr<T> & t, unsigned n) {
  if(!t) return;
  is->read(reinterpret_cast<char*>(t.p), sizeof(T)*n);
}

template<class T>
void ICacheFile::Read(TPtrVec<T> & v, unsigned n) {
  for(unsigned i = 0; i < v.size(); i++)
    Read(v[i], n);
}
