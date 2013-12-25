#pragma once

#include"main.h"
#include "ci.h"

class Cell;

class Integral {

  public:

    static Integral* Instance() {
      static Integral integral_;
      return &integral_;
    }

    void Iteration();

    void FillInput(Cell* cell_gas1, Cell* cell_gas2);

    void GetInput(Cell* cell_gas1, Cell* cell_gas2);

    void Generate( double time_step, double m1, double m2,
      ci::Particle p1, ci::Particle p2 );

  private:
    Integral();
    ~Integral() {};

    ci::HSPotential potential;
    ci::Particle particle;
    int radius;
    double a;
    int*** xyz2i;
    double* input_for_ci1;
    double* input_for_ci2;
};

#define INTEGRAL Integral::Instance()
