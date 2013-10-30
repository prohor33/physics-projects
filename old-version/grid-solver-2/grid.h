#ifndef _GRID_H_
#define _GRID_H_

#include"main.h"
#include"cell.h"

#define NO_SYMMETRY 0
#define QUARTER_SYMMETRY 1

#define START_MAIN_INFO 1
#define END_MAIN_INFO 2
#define START_CELL 3
#define END_CELL 4
#define END_FILE 5

struct MAIN_INFO{
	int size_x, size_y;
	int gaps_start_x, gaps_start_y;
	int gaps_quantity;
	int d, h;
	int depth_of_the_gap;
	int sp_start_x, sp_start_y;
	int sp_size_x, sp_size_y;
	int l_sp;
};

struct CELL{
	int coord_x, coord_y;
	double T_start;
	double h_x, h_y;
	double n;
	int type;
};

class Grid {
 public:
	 Grid(void){};
	void NewGridFromByteFile( char* f_name,  int speed_quantity, Cell*** &Cells, int gas_n );
  void MakeStep( Cell*** Cells );
	void SetInitialConditions( Cell*** Cells, ... );
  Cell*** Cells1;
	Cell*** Cells2;
  int size_x;
  int size_y;
  int FLAG_USE_SYMMETRY;
};

extern Grid* Grid_;

class Symmetry {
 public:
   Symmetry( void ){};
  void ReflectGrid( Cell*** Cells );
  void ReflectHalfGrid( Cell*** Cells );
};

extern Symmetry* Symmetry_;

#endif