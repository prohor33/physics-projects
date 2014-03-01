#ifndef _PARALLEL_H_
#define _PARALLEL_H_

#include "main.h"

class Parallel {

 public:

  static Parallel* Instance() {
    static Parallel Parallel_;
    return &Parallel_;
  }

  void InitializeMPI(int argc, char *argv[]);

  void FinalizeMPI();

  void ExchangeEdgeZone(sep::SpeedType sp_type);

  void SendRecieveCellsBlock(vector<int>& start_s, vector<int>& size,
                             int shift, sep::Axis axis,
                             sep::SpeedType sp_type, int dest);

  void SendRecieveCell(vector<int> cell_s_c,
                                 vector<int> cell_r_c,
                                 sep::SpeedType sp_type, int dest);

 private:

  Parallel() {};
  ~Parallel() {};

};

#define PARALLEL Parallel::Instance()

#endif  // _PARALLEL_H_
