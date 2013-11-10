#ifndef _INITIALIZER_H_
#define _INITIALIZER_H_


class Initializer {

 public:

  static Initializer* Instance() {
    static Initializer Initializer_;
    return &Initializer_;
  }

  void InitializeSolver();

 private:

  Initializer() {};
  ~Initializer() {};


};

#define INITIALIZER Initializer::Instance()

#endif  // _INITIALIZER_H_
