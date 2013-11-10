#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include "main.h"

class Cell;

class Parameters {

  friend class Cell;

 public:

  static Parameters* Instance() {
    static Parameters parameters_;
    return &parameters_;
  }

  void SetMolMass(
      double mol_mass_gas1,
      double mol_mass_gas2) {
    mol_mass_gas1_ = mol_mass_gas1;
    mol_mass_gas2_ = mol_mass_gas2;
  }

  void SetTimeStep(double time_step) {
    time_step_ = time_step;
  }

 protected:

  Parameters() {};
  ~Parameters() {};

  double mol_mass_gas1_, mol_mass_gas2_;

  double time_step_;
  vector<double> velocity_;
  sep::Limiter limiter_;

};

#define PARAMETERS Parameters::Instance()

#endif // _PARAMETERS_H_
