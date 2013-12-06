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

  void SetLimiter(sep::Limiter limiter) {
    limiter_ = limiter;
  }

  void SetProcessID(int id) { process_id_ = id; }

  int GetProcessID() {
    return process_id_;
  }

  bool GetSecondGasIsActive() { return second_gas_is_active_; };

  void SetSecondGasIsActive(bool x) { second_gas_is_active_ = x; };

  double time_step() { return time_step_; };

  void SetUseCheckingMassConservation(bool x) {
    use_checking_mass_conservation_ = x;
  }
  bool GetUseCheckingMassConservation() {
    return use_checking_mass_conservation_;
  }

  void SetUseZAxis(bool x) {
    use_z_axis_ = x;
  }
  bool GetUseZAxis() {
    return use_z_axis_;
  }

 protected:

  Parameters();
  ~Parameters() {};

  double mol_mass_gas1_, mol_mass_gas2_;

  double time_step_;
  vector<double> velocity_;
  sep::Limiter limiter_;

  int process_id_; // for MPI

  bool second_gas_is_active_; // second gas flag

  map<int, vector<int> > s_coord_map_1d_to_3d_;
  map<vector<int>, int> s_coord_map_3d_to_1d_;

  bool use_checking_mass_conservation_;

  bool use_z_axis_;
};

#define PARAMETERS Parameters::Instance()

#endif // _PARAMETERS_H_
