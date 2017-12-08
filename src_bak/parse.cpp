
#include "TStat.h"
#include <map>
#include <cstdio>

int main(int argc, char * argv[]) {
  std::map< unsigned /** thr */, TStat > stat;
  FILE * pFile = fopen("log4.dat", "r");
  while(pFile && !ferror(pFile) && !feof(pFile)) {
    unsigned thr;
    double time;
    fscanf(pFile, "Threads = %u, fit call time = %lf\n", &thr, &time);
//     if(feof(pFile)) break;
//     printf("%3u%8.2lf\n", thr, time);
    stat[thr].Add(time/60.);
  }
  fclose(pFile);
  pFile = fopen("log5.dat", "w");
  for(auto it = stat.begin(); it != stat.end(); it++) {
    fprintf(pFile, "%3u%8.4f%7.4lf\n", (*it).first, (*it).second.Mean(), (*it).second.StDev());
  }
  fclose(pFile);
}
