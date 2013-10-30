#include"main_info.h"

extern MainInfo* MainInfo_ = NULL;

MainInfo::MainInfo( void ) {	
  return;
}

void MainInfo::Setup( void ) {
	double T = 1.0f;	// примерно порядок
	//if( MainInfo_->mol_mass_2 > MainInfo_->mol_mass_1 )
	//	p_cut = sqrt( MainInfo_->mol_mass_2 * T );
	//else
	//	p_cut = sqrt( MainInfo_->mol_mass_1 * T );
	p_cut = 4.8f*2;
	// импульс
	delta_p = 2.0f * p_cut / speed_quantity;
  if( speed_quantity%2 != 0 ) {
    printf("Error speed quantity must be odd\n");
    return;
  }
  p = new double[speed_quantity];
  for( int i=0; i<speed_quantity; i++ ) {
    p[i] = -p_cut + p_cut*2.0f*i/( speed_quantity-1 );
  }

	FLAG_limiter = 3; // 1 - MC

  g_speed = new double**[speed_quantity];
  for(int i=0; i<speed_quantity; i++) {
    g_speed[i] = new double*[speed_quantity];
    for(int j=0; j<speed_quantity; j++) {
      g_speed[i][j] = new double[speed_quantity];
    }
  }
	main_info = new MAIN_INFO;
	return;
}