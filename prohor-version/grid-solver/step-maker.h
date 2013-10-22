#ifndef _STEP_MAKER_H_
#define _STEP_MAKER_H_

class StepMaker {
 public:
  StepMaker* instance() {
    static StepMaker StepMaker_;
    return &StepMaker_;
  }
  enum Axis {X, Y, Z};
  void makestep(Axis axis, double dt) {};
 private:
  StepMaker() {};
  ~StepMaker() {};
};

#define SM StepMaker::instance()

#endif  // _STEP_MAKER_H_
