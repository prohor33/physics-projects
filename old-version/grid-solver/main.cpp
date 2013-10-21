#include"main.h"
#include"main_info.h"
#include"grid.h"
#include"out_macro_parameters.h"
#include "ci.hpp"
#include"edge_conditions.h"
#include"integral.h"

void main() {
	MainInfo_ = new MainInfo(); 
  //MainInfo_->speed_quantity = 20;
	MainInfo_->speed_quantity = 16;  
	printf( "Enter quantity of iterations: " );
	scanf( "%i", &MainInfo_->quantity_of_iteration );
	if( MainInfo_->quantity_of_iteration < 0 ) {
		printf("As you wish\n");
		return;
	}
	printf( "Enter each print: " );
	scanf( "%i", &MainInfo_->each_print );
	if( MainInfo_->each_print < 1 )
		MainInfo_->each_print = 1;
  MainInfo_->FLAG_use_cut_sphere = true;
	MainInfo_->FLAG_use_only_sphere_data = true;
	MainInfo_->FLAG_use_edge_conditions = true;
	MainInfo_->FLAG_use_initial_stream = false;	
  Grid_ = new Grid;
	MainInfo_->knudsen_number = 0.5f; // L = 2*lambda; lambda/L = 1/2

	MainInfo_->mol_mass_1 = 1.0f;
	MainInfo_->mol_mass_2 = 2.0f;
	MainInfo_->Setup();
	Grid_->NewGridFromByteFile( "data/input/Japanese.grid", MainInfo_->speed_quantity, Grid_->Cells1, 1 );	
	Grid_->NewGridFromByteFile( "data/input/Japanese.grid", MainInfo_->speed_quantity, Grid_->Cells2, 2 );	
	MainInfo_->initial_stream = 4.0f/QUAD(MainInfo_->main_info->l_sp-1)*
		MainInfo_->p_cut/MainInfo_->mol_mass_2/20.0f;
	Grid_->SetInitialConditions( Grid_->Cells1 );
	Grid_->SetInitialConditions( Grid_->Cells2 );
	OutMacroParameters1_ = new OutMacroParameters( Grid_->Cells1, 1 );
	OutMacroParameters2_ = new OutMacroParameters( Grid_->Cells2, 2 );
	OutMacroParameters1_->FillConsuptionAreas();
	OutMacroParameters2_->FillConsuptionAreas();
	EdgeCondition1_ = new EdgeCondition( Grid_->Cells1, 1 );
	EdgeCondition2_ = new EdgeCondition( Grid_->Cells2, 2 );
	EdgeCondition1_->FillEdgeConditionAreas();
	EdgeCondition2_->FillEdgeConditionAreas();
	EdgeCondition1_->FillEdgeAreas();
	EdgeCondition2_->FillEdgeAreas();

	MainInfo_->time_step =  1.0f/ MainInfo_->knudsen_number / 100.0f / ( MainInfo_->p_cut/1.0f /*cut*/ ) ;  // tau
		
	Integral_ = new Integral();
	Integral_->FLAG_use_ci = true;

  clock_t start=clock();
	// пошел главный цикл
  for( int i=0; i<MainInfo_->quantity_of_iteration; i++ ) {		

    if(i%MainInfo_->each_print == 0) {
      OutMacroParameters1_->InfoOut(start, i);
      OutMacroParameters1_->OutConcentration();
      OutMacroParameters1_->OutTemperature();
			OutMacroParameters1_->VelocityOut();      
			OutMacroParameters1_->ConsuptionOut();
			OutMacroParameters1_->OutPressure();
      OutMacroParameters1_->OutGlobalParameters();

      OutMacroParameters2_->InfoOut(start, i);
      OutMacroParameters2_->OutConcentration();
      OutMacroParameters2_->OutTemperature();
			OutMacroParameters2_->VelocityOut();      
			OutMacroParameters2_->ConsuptionOut();
			OutMacroParameters2_->OutPressure();
      OutMacroParameters2_->OutGlobalParameters();
    }
    OutMacroParameters1_->PrintPercent(start, i);    
		Grid_->MakeStep( Grid_->Cells1 );
		Grid_->MakeStep( Grid_->Cells2 );
		Integral_->Iteration();
  }  

  return;
}