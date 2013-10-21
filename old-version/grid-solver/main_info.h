#ifndef _MAIN_INFO_H_
#define _MAIN_INFO_H_

#include"main.h"
#include"grid.h"

class MainInfo {
public:
  MainInfo( void );  
	void Setup( void );
	int speed_quantity;
  int FLAG_limiter; // 1 - MC
  double*** g_speed;
  int quantity_of_iteration;
  int FLAG_use_cut_sphere;
	int FLAG_use_only_sphere_data;
	int FLAG_use_edge_conditions;
	int FLAG_use_initial_stream;
	double knudsen_number;
	double time_step;
	int each_print;
	double p_cut; // импульс
	double* p;
	double delta_p;
	double initial_stream;
	double mol_mass_1, mol_mass_2;
	MAIN_INFO* main_info;
};

extern MainInfo* MainInfo_;

#endif