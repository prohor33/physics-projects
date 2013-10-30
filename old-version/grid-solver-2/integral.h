#pragma once
#include"main.h"
#include"ci.hpp"
#include"main_info.h"

class Integral {
public:
	Integral(void);
	void Iteration( void );
	void FillInput( int x, int y );
	void GetInput( int x, int y );
	void Generate( double time_step, double m1, double m2,  ci::Particle p1, ci::Particle p2 );
	ci::HSPotential potential;
	ci::Particle particle;
	int radius;
	double a;
  int*** xyz2i;
	double* input_for_ci1;
	double* input_for_ci2;
	bool FLAG_use_ci;
};

extern Integral* Integral_;