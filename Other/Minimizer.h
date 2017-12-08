
#pragma once

class ObjectiveFunction;

class Minimizer {
protected:
  ObjectiveFunction * objFunc;
public:
  Minimizer(): objFunc(0) { }
  Minimizer(ObjectiveFunction * f): objFunc(f) { }
  virtual ~Minimizer() { }
  virtual void SetObjFunc(ObjectiveFunction * f) { objFunc = f; }
  virtual void Minimize() = 0;
};

class TVirtualFitter;

class ROOTMinimizer: public Minimizer {
protected:
  static void FCN(int & n_par, double * gradients, double & ret_val, double * par, int flag);
  static TVirtualFitter * gFitter;
  static ObjectiveFunction * gObjFunc;
public:
  double tolerance;
  enum Strategy {FUMILI, SIMPLEX, MIGRAD, MIGRAD_G};
  static Strategy gStrategy;
  ROOTMinimizer(): tolerance(0.01) { }
  ROOTMinimizer(Strategy s): tolerance(0.01) { gStrategy = s; }
  ROOTMinimizer(ObjectiveFunction * f): Minimizer(f), tolerance(0.01) { }
  ROOTMinimizer(Strategy s, ObjectiveFunction * f): Minimizer(f), tolerance(0.01) { gStrategy = s; }
  virtual ~ROOTMinimizer() { }
  virtual void Minimize() { Minimize(0); }
  virtual void Minimize(unsigned max_calls); // 0 -- default for current algorithm
};
