
#include "TResonanceSet.h"
#include "TCacheFile.h"
#include <fstream>
#include <sstream>

ICacheFile::ICacheFile(string name): type(kFile), buffer(0) {
  is = new std::ifstream(name, std::ios::in | std::ios::binary);
}

ICacheFile::ICacheFile(string * buf): type(kBuffer), buffer(buf) {
  is = new std::istringstream(*buf, std::ios::in | std::ios::binary);
}

ICacheFile::~ICacheFile() {
  if(type == kFile)
    static_cast<std::ifstream*>(is)->close();
  delete is;
}

OCacheFile::OCacheFile(string name): type(kFile), buffer(0) {
  os = new std::ofstream(name, std::ios::out | std::ios::binary);
}

OCacheFile::OCacheFile(string * buf): type(kBuffer), buffer(buf) {
  os = new std::ostringstream(*buf, std::ios::out | std::ios::binary);
}

OCacheFile::~OCacheFile() {
  if(type == kFile) {
    static_cast<std::ofstream*>(os)->close();
  } else {
    *buffer = static_cast<std::ostringstream*>(os)->str();
  }
  delete os;
}

void OCacheFile::Write(string & s) {
  unsigned l = s.length()+1;
  os->write(reinterpret_cast<char*>(&l), sizeof(l));
  os->write(s.c_str(), sizeof(char)*l);
}

void ICacheFile::Read(string & s) {
  unsigned l;
  is->read(reinterpret_cast<char*>(&l), sizeof(l));
  char * c = new char[l];
  is->read(c, sizeof(char)*l);
  s = c;
  delete [] c;
}

void OCacheFile::Write(TResonanceSet * set) {
  set->Write(this);
}

void ICacheFile::Read(TResonanceSet * set) {
  set->Read(this);
}

OCacheFile::operator bool() {
  if(type == kFile) {
    std::ofstream * ofs = static_cast<std::ofstream*>(os);
    return *ofs && ofs->is_open();
  }
  return *os;
}

ICacheFile::operator bool() {
  if(type == kFile) {
    std::ifstream * ifs = static_cast<std::ifstream*>(is);
    return *ifs && ifs->is_open();
  }
  return *is;
}
