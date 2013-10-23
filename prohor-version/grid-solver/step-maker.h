#ifndef _STEP_MAKER_H_
#define _STEP_MAKER_H_

#include "main.h"

class StepMaker {
 public:
  static StepMaker* Instance() {
    static StepMaker step_maker_;
    return &step_maker_;
  }

  void MakeStep(sep::Axis axis, double dt) {};

 private:
  StepMaker() {};
  ~StepMaker() {};
};

#define STEP_MAKER StepMaker::Instance()

#endif  // _STEP_MAKER_H_
