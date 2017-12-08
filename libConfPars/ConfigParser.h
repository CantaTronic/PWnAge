
#pragma once

#include "TFitPar.h"
#include "TResonanceSet.h"
#include <sstream>
#include <string>

class TInputFiles;

class ConfigParser {
public:
   /*!A constructor with in-built parsing
   *
   * Creates an instance of the class and parses config file
   *
   * \param [in]  ConfigFileName name of the config file
   * \param [out] _r vector of resonances to be filled
   */
  ConfigParser(std::string configFileName, TResonanceSet * set, TInputFiles * inputFiles);
  /*!
   * A destructor
   *
   * Clears memory
   */
  ~ConfigParser() { }
  
  ConfigParser(){};
  
  std::string test (std::string str) {return "test "+str;}
private:
  /*!Method for finding TFitPar type parameter
   *
   * The string in the config file looks like:
   * FitPar1 = starting_value lower_limit upper_limit
   * any number of parameters in the config string except the first one can be omitted
   * in this case they are replaced by default ones
   *
   * \param [in] _v the value to return if no parameter with such name is found in the config file
   * \param [in] _vmin the lower limit to return if not given in the config file
   * \param [in] _vmax the upper limit to return if not given in the config file
   * \param [in] _vmax the step size to return if not given in the config file
   * \return     value of the parameter with the given name
   */
  void GetFitPar(TFitPar p, double _v = 0);
  void GetProdPhase(TFitPar p, double _v = 0);
  void GetSymmetryMultiplier(TResonance & res);
//   void getByFlag(std::string flag, std::string name, ParToGet param);
  template<class T>
  void Get(T & v);
  template<class T>
  T Get();
  std::istringstream iss; //! line currently being parsed
};
