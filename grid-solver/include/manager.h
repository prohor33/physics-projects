#ifndef _MANAGER_H_
#define _MANAGER_H_


class Manager {

 public:

  static Manager* Instance() {
    static Manager Manager_;
    return &Manager_;
  }

  void Initialize(int argc, char *argv[]);

  void InitializeSolver();

  void SetParameters();

 private:

  Manager() {};
  ~Manager() {};

};

#define MANAGER Manager::Instance()

#endif  // _MANAGER_H_
