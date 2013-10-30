#ifndef _OUTMACROPARAMETERS_H__
#define _OUTMACROPARAMETERS_H__

#include"main_info.h"
#include"grid.h"
#include"cell.h"

#include<math.h>
#include<time.h>

class OutMacroParameters {
 public:
  OutMacroParameters( Cell*** Cells, int gas_index );
  FILE* file_concentration;
  FILE* file_temperature;
  FILE* file_stream_of_heap;
	FILE* gas_velocity;
  FILE* file_info;
  FILE* file_global_info;
  FILE* total_concentration_file;
	FILE* gas_consuption[200];
  int number_of_file;
  void OutConcentration(void);
  void OutTemperature(void);
  void StreamOut(void);
	void VelocityOut(void);
  void InfoOut(clock_t start, int main_circle);
	void ConsuptionOut(void);
  void PrintPercent(clock_t start, int main_circle);
  void OutGlobalParameters(void);
	bool FillConsuptionAreas( void );	
  double max_concentration, min_concentration;
  double max_temperature, min_temperature;
	double max_mod_gas_velocity, min_mod_gas_velocity;
	Cell*** Cells;
	int gas_index;
	struct CONSUPTION_AREA {
		enum {
			HORISONTAL, VERTICAL
		} orientation;
		COORD_int coord;
		int size;
		string name;
	};
	void ConsuptionAreaOut( CONSUPTION_AREA* area, int num );
	CONSUPTION_AREA** consuptions_areas;
	int cons_areas_quantity;
};

extern OutMacroParameters* OutMacroParameters1_;
extern OutMacroParameters* OutMacroParameters2_;

#endif