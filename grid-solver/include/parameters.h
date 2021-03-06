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

  void Initialize();

  void SetMolMass(
      double mol_mass_gas1,
      double mol_mass_gas2) {
    mol_mass_gas1_ = mol_mass_gas1;
    mol_mass_gas2_ = mol_mass_gas2;
  }

  double GetMolMass(int gas_numb) {
    switch(gas_numb) {
      case 0:
        return mol_mass_gas1_;
      case 1:
        return mol_mass_gas2_;
      default:
        return -1;
    }
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

  void SetProcessesQ(int p_q) {
    if (p_q != 1 && p_q%2 != 0) {
      cout << "Error: process quantity should be odd number, not " <<
        p_q << endl;
    }
    processes_q_ = p_q;
  }

  int GetProcessesQ() {
    return processes_q_;
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

  // we do not need call this in main
  // because it's invoes in manager.cpp
  // automatically
  void SetUseParallelComputing(bool x) {
    use_parallel_computing_ = x;
  }
  bool GetUseParallelComputing() {
    return use_parallel_computing_;
  }

  void SetGridFromInputFile(bool x) {
    use_grid_from_input_file_ = x;
  }
  bool GetGridFromInputFile() {
    return use_grid_from_input_file_;
  }

  void SetUseCollisionIntegral(bool x) {
    use_collision_integral_ = x;
  }
  bool GetUseCollisionIntegral() {
    return use_collision_integral_;
  }

  void SetSpeedQuantity(int x) {
    speed_quantity_ = x;
  }
  int GetSpeedQuantity() {
    return speed_quantity_;
  }

  void SetPCut(double x) {
    p_cut_ = x;
  }
  double GetPCut() {
    return p_cut_;
  }

  inline double& Gamma(vector<int>& coord) {
    return gamma_[coord[sep::INDEX]];
  }

  void SetInputGridFilename(const string& filename) {
    input_grid_filename_ = filename;
  }
  const string& GetInputGridFilename() {
    return input_grid_filename_;
  }

  void SetLogType(sep::LogType ltype);

  void SetIterationQ(int q) {
    iteration_q_ = q;
  }
  int GetIterationQ() {
    return iteration_q_;
  }

  void SetUseFlowKeeper(bool x) {
    use_flow_keeper_ = x;
  }
  bool GetUseFlowKeeper() {
    return use_flow_keeper_;
  }

  void SetUseStartTemperature(bool x) {
    use_start_temperature_ = x;
  }
  bool GetUseStartTemperature() {
    return use_start_temperature_;
  }

  void SetT1T2(double T1, double T2) {
    T1_ = T1; T2_ = T2;
  }
  double GetT1() const { return T1_; }
  double GetT2() const { return T2_; }

  void SetStartFlow(double x) {
    start_flow_ = x;
  }
  double GetStartFlow() {
    return start_flow_;
  }
  void SetConcentrations(double n1, double n2, double n3,  double n4) {
    n_[0] = n1; n_[1] = n2; n_[2] = n3; n_[3] = n4;
  }
  double GetConcentration(int i) {
    if (i < 0 || i > 3)
      return -1.0;
    return n_[i];
  }
  const vector<int>& GetGridSize();
  vector<int> GetWholeGridSize();
  void OutTime();
protected:

  Parameters();
  ~Parameters() {};

  double mol_mass_gas1_, mol_mass_gas2_;

  double time_step_;
  vector<double> velocity_;
  sep::Limiter limiter_;

  int process_id_; // for MPI
  int processes_q_; // too

  bool second_gas_is_active_; // second gas flag

  vector<vector<int> > s_coord_map_1d_to_3d_;
  map<vector<int>, int> s_coord_map_3d_to_1d_;

  bool use_checking_mass_conservation_;
  bool use_z_axis_;
  bool use_parallel_computing_;
  bool use_grid_from_input_file_;
  bool use_collision_integral_;
  bool use_flow_keeper_;
  bool use_start_temperature_;
  int speed_quantity_;
  double p_cut_;
  double T1_, T2_;
  double start_flow_;
  double n_[4];

  vector<double> gamma_;

  string input_grid_filename_;

  streambuf* cout_backup_buf_;
  streambuf* log_file_buf_;
  ofstream log_file_str_;
  sep::LogType log_type_;

  int iteration_q_;
};

#define PARAMETERS Parameters::Instance()

#endif // _PARAMETERS_H_
