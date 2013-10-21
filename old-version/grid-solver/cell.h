#ifndef _CELL_H_
#define _CELL_H_

#include"main.h"

class Cell {
 public:
  Cell( int speed_quantity );
  virtual void CountHalfCell_with_x_step( double like_a_tau );
  virtual void CountHalfCell_with_y_step( double like_a_tau );
  virtual void CountCell_with_x_step( double like_a_tau );
  virtual void CountCell_with_y_step( double like_a_tau );
  double GetLimiter_x( int x, int y, int z );
  double GetLimiter_y( int x, int y, int z );
	double GetMolMass( void );
	double GetP( int i, ... );
	double GetP( int i, int j, int k, ... );// return sqrt(x*x+y*x+z*z)
	double GetP2( int i, int j, int k, ... );// return x*x+y*x+z*z
  double*** future_SPEED_CUBE;  // надо будет менять указатели
  double*** real_SPEED_CUBE;  // 
  double*** SPEED_CUBE_half;  // 
  // нужны ссылки на соседей!
  Cell* Cell_x_plus;
  Cell* Cell_x_minus; // Down - это 0
  Cell* Cell_y_plus;
  Cell* Cell_y_minus;
  int coord_x;
  int coord_y;
  double T_st_x;  // -1 for ordinary cell
  double T_st_y;  // -1 for ordinary cell
	double T_start;
	double h_x;
	double h_y;
	double n; // concentration
	int gas_n; // gas number
  int type;
};

class OrdinaryCell: public Cell {
 public:
  OrdinaryCell( int speed_quantity );
};

class LeftBorderCell: public Cell {
 public:
  LeftBorderCell( int speed_quantity );    
  void CountHalfCell_with_x_step( double like_a_tau );  
};

class RightBorderCell: public Cell {
 public:
  RightBorderCell( int speed_quantity );    
  void CountHalfCell_with_x_step( double like_a_tau );
};

class UpBorderCell: public Cell {
 public:
  UpBorderCell( int speed_quantity );    
  void CountHalfCell_with_y_step( double like_a_tau );
};

class DownBorderCell: public Cell {
 public:
  DownBorderCell( int speed_quantity );    
  void CountHalfCell_with_y_step( double like_a_tau );
};

class UpLeftBorderCell: public Cell {
 public:
  UpLeftBorderCell( int speed_quantity );    
  void CountHalfCell_with_x_step( double like_a_tau );
  void CountHalfCell_with_y_step( double like_a_tau );
};
class DownLeftBorderCell: public Cell {
 public:
  DownLeftBorderCell( int speed_quantity );    
  void CountHalfCell_with_x_step( double like_a_tau );
  void CountHalfCell_with_y_step( double like_a_tau );
};
class UpRightBorderCell: public Cell {
 public:
  UpRightBorderCell( int speed_quantity );    
  void CountHalfCell_with_x_step( double like_a_tau );
  void CountHalfCell_with_y_step( double like_a_tau );
};
class DownRightBorderCell: public Cell {
 public:
  DownRightBorderCell( int speed_quantity );    
  void CountHalfCell_with_x_step( double like_a_tau );
  void CountHalfCell_with_y_step( double like_a_tau );
};

class FakeCell: public Cell {
 public:
  FakeCell( int speed_quantity );    
  void CountHalfCell_with_x_step( double like_a_tau );
  void CountHalfCell_with_y_step( double like_a_tau );
  void CountCell_with_x_step( double like_a_tau );
  void CountCell_with_y_step( double like_a_tau );
};

double sign( double x );
double module( double x );
double min_( double x, double y );
double max_( double x, double y );

//double square_of_speed( int x, int y, int z );
//double mod_of_speed( int x, int y, int z );

void CountHalfCell_with_x_step_LeftBorderCell( Cell* c, double like_a_tau );
void CountHalfCell_with_x_step_RightBorderCell( Cell* c, double like_a_tau );
void CountHalfCell_with_y_step_DownBorderCell( Cell* c, double like_a_tau );
void CountHalfCell_with_y_step_UpBorderCell( Cell* c, double like_a_tau );

#endif