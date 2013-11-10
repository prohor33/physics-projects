#ifndef _OUT_RESULT_H_
#define _OUT_RESULT_H_

#include "main.h"


class OutResult {

 public:

  enum OutType {
    OUT_FOR_PYTHON
  };

  static OutResult* Instance() {
    static OutResult OutResult_;
    return &OutResult_;
  }

  void OutParameters();

  void SetOutputType(OutType output_type) {
    output_type_ = output_type;
  }

 private:

  OutResult() {};
  ~OutResult() {};

 protected:
  OutType output_type_;

};

#define OUT_RESULT OutResult::Instance()

#endif  // _OUT_RESULT_H_
