#ifndef _OUT_RESULT_H_
#define _OUT_RESULT_H_

#include "main.h"


class CellParameters {

public:

  CellParameters() {};

  CellParameters(
      vector<int> coord,
      double T,
      double n
      ) :
      coord(coord),
      T(T),
      n(n)
      {};

  ~CellParameters() {};

  vector<int> coord;
  double T;
  double n;
};

class OutResult {

 public:

  enum OutType {
    OUT_FOR_PYTHON
  };

  static OutResult* Instance() {
    static OutResult OutResult_;
    return &OutResult_;
  }

  void OutParameters(sep::GasNumb gas_numb);

  void ProcessParameters(sep::GasNumb gas_numb);

  void SetOutputType(OutType output_type) {
    output_type_ = output_type;
  }

  void CheckMassConservation(sep::GasNumb gas_numb);

 private:

  OutResult() :
    whole_mass_(0) {};
  ~OutResult() {};

 protected:

  OutType output_type_;

  vector<CellParameters> parameters_[2];  // two gases

  double whole_mass_; // use for checking mass conservation law
};

#define OUT_RESULT OutResult::Instance()

#endif  // _OUT_RESULT_H_
