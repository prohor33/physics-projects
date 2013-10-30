#include"out_macro_parameters.h"
#include"main_info.h"
#include"main.h"

OutMacroParameters* OutMacroParameters1_ = NULL;
OutMacroParameters* OutMacroParameters2_ = NULL;

string mmmy_to_string(long double x) {
	std::stringstream ss;
	ss << x;
	std::string s(ss.str());
	return s;
}

OutMacroParameters::OutMacroParameters( Cell*** Cells, int gas_index ) {
	this->Cells = Cells;
	this->gas_index = gas_index;
  number_of_file = 0;
  max_concentration = 0.0f;
  min_concentration = 1000.0f;
  max_temperature = 0.0f;
  min_temperature = 1000.0f;
	max_mod_gas_velocity = 0.0f;
	min_mod_gas_velocity = 1000.0f;
  total_concentration_file = NULL;
	for( int i=0; i<200; i++ )
		gas_consuption[i] = NULL;
  return;
}

void OutMacroParameters::OutConcentration(void) {
	char file_name[50];
  sprintf(file_name, "data/output/gas %i/concentration%i.txt", gas_index, number_of_file);
  file_concentration = fopen(file_name, "w");  
  double concentration;
  double total_concentration = 0.0f;
  double local_max_concentration = 0.0f;
  double local_min_concentration = 1000.0f;
  int sym_size_x, sym_size_y;
  int sym_x, sym_y;
  switch( Grid_->FLAG_USE_SYMMETRY ) {
  case NO_SYMMETRY:
    sym_size_x = Grid_->size_x-1;
    sym_size_y = Grid_->size_y-1;
    break;
  case QUARTER_SYMMETRY:
    sym_size_x = Grid_->size_x/2.0f;
    sym_size_y = Grid_->size_y/2.0f;
    break;
  }
  for(int y=0; y<Grid_->size_y; y++) {
    for(int x=0; x<Grid_->size_x; x++) {    
			if( !Cells[x][y] || Cells[x][y]->type == 9 ) {
				fprintf(file_concentration, "%i %i %f\n", x, y, 0.0f);
			}
			else {
      sym_x = x;
      sym_y = y;
      if( Grid_->FLAG_USE_SYMMETRY == QUARTER_SYMMETRY ) {
					if( x > (sym_size_x-1) ) {
						sym_x = Grid_->size_x-1-x;
					}
					if( y > (sym_size_y-1) ) {
						sym_y = Grid_->size_y-1-y;
					}
				}
				concentration = 0.0f;		
				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							if( Cells[sym_x][sym_y]->GetP(i,j,k) > MainInfo_->p_cut &&
								MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
							}
							else {
								concentration += Cells[sym_x][sym_y]->real_SPEED_CUBE[i][j][k];
							}
						}
					}
				}
				fprintf(file_concentration, "%i %i %f\n", x, y, concentration);
				total_concentration += concentration;
				if(concentration > local_max_concentration)
					local_max_concentration = concentration;
				if(concentration < local_min_concentration)
					local_min_concentration = concentration;
			}
    }
  }
  if( local_max_concentration > max_concentration )
    max_concentration = local_max_concentration;
  if( local_min_concentration < min_concentration )
    min_concentration = local_min_concentration;
  //printf("total concentration = %f\n", total_concentration);
  if(total_concentration_file == NULL) {
		if( gas_index == 1 )
			total_concentration_file = fopen("data/output/gas 1/total_concentration_file.txt", "w");
		else
			total_concentration_file = fopen("data/output/gas 2/total_concentration_file.txt", "w");
	}
  fprintf(total_concentration_file, "%f\n", total_concentration);
  fclose(file_concentration);

  char file_name2[50];
  sprintf(file_name2, "data/output/gas %i/max_and_min_conc_and_temp%i.txt", gas_index, number_of_file);  
  file_info = fopen( file_name2, "w" );
  fprintf( file_info, "%f\n", local_max_concentration );
  fprintf( file_info, "%f\n", local_min_concentration );
  fclose( file_info );
  return;
}

void OutMacroParameters::PrintPercent(clock_t start, int main_circle){
	// ���� �� ������ ���������� ����� �������
	int last_procent = -1;
	int procent;
	double workTime;
	int days, hours, minutes, seconds;

	procent = (int)(( (float)main_circle )*100.0f/( (float)MainInfo_->quantity_of_iteration ));
	if( last_procent != procent ){
		printf("\r                                                                 ");
		printf("\r%i%%\t", procent);
		last_procent = procent; 

		clock_t finish=clock();    
		workTime=(double)(clock() - start) / CLOCKS_PER_SEC;
		double sp_workTime = workTime;
		if( procent == 0 ) {
			printf("Time passed: %.0fs\tTime left: can't tell yet", workTime);
			return;
		}
		days = workTime/3600/24;
		workTime -= days*3600*24;
		hours = workTime/3600;
		workTime -= hours*3600;
		minutes = workTime/60;
		workTime -= minutes*60;
		seconds = workTime;
		bool sp_FLAG_its_all_right = false;

		printf( "Time passed: " );
		if( days > 0 ) {
			printf( "%id ", days );		sp_FLAG_its_all_right = true;
		}
		if( hours > 0 || sp_FLAG_its_all_right ) {
			printf( "%ih ", hours );		sp_FLAG_its_all_right = true;
		}
		if( minutes > 0 || sp_FLAG_its_all_right ) {
			printf( "%im ", minutes );		sp_FLAG_its_all_right = true;
		}
		if( seconds > 0 || sp_FLAG_its_all_right ) {
			printf( "%is ", seconds );		sp_FLAG_its_all_right = true;
		}		

		workTime = sp_workTime * ( 100.0f - procent ) / procent;
		days = workTime/3600/24;
		workTime -= days*3600*24;
		hours = workTime/3600;
		workTime -= hours*3600;
		minutes = workTime/60;
		workTime -= minutes*60;
		seconds = workTime;

		sp_FLAG_its_all_right = false;
		printf( "Time left: " );
		if( days > 0 ) {
			printf( "%id ", days );		sp_FLAG_its_all_right = true;
		}
		if( hours > 0 || sp_FLAG_its_all_right ) {
			printf( "%ih ", hours );		sp_FLAG_its_all_right = true;
		}
		if( minutes > 0 || sp_FLAG_its_all_right ) {
			printf( "%im ", minutes );		sp_FLAG_its_all_right = true;
		}
		if( seconds > 0 || sp_FLAG_its_all_right ) {
			printf( "%is ", seconds );		sp_FLAG_its_all_right = true;
		}
	}
	return;
}

void OutMacroParameters::OutTemperature(void) {
  char file_name[50];
  sprintf(file_name, "data/output/gas %i/temperature%i.txt", gas_index, number_of_file);
  file_temperature = fopen(file_name, "w");
  //file_temperature = fopen("temperature.txt", "w");
	double n;
  double u_x, u_y, u_z;
  double temperature;
  double local_max_temperature = 0.0f;
  double local_min_temperature = 1000.0f;
  int sym_size_x, sym_size_y;
  int sym_x, sym_y;
  switch( Grid_->FLAG_USE_SYMMETRY ) {
  case NO_SYMMETRY:
    sym_size_x = Grid_->size_x-1;
    sym_size_y = Grid_->size_y-1;
    break;
  case QUARTER_SYMMETRY:
    sym_size_x = Grid_->size_x/2.0f;
    sym_size_y = Grid_->size_y/2.0f;
    break;
  }
  for(int y=0; y<Grid_->size_y; y++) {
    for(int x=0; x<Grid_->size_x; x++) {
			if( !Cells[x][y] || Cells[x][y]->type == 9 ) {
				fprintf(file_temperature, "%i %i %f\n", x, y, 0.0f);
			}
			else {
				sym_x = x;
				sym_y = y;
				if( Grid_->FLAG_USE_SYMMETRY == QUARTER_SYMMETRY ) {
					if( x > (sym_size_x-1) ) {
						sym_x = Grid_->size_x-1-x;
					}
					if( y > (sym_size_y-1) ) {
						sym_y = Grid_->size_y-1-y;
					}
				}
				n = 0.0f;
				u_x = 0.0f;
				u_y = 0.0f;
				u_z = 0.0f;
				temperature = 0.0f;
				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							if( Cells[sym_x][sym_y]->GetP(i,j,k) > MainInfo_->p_cut &&
								MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
							}
							else {
								n += Cells[sym_x][sym_y]->real_SPEED_CUBE[i][j][k];

								u_x += Cells[sym_x][sym_y]->real_SPEED_CUBE[i][j][k]*
									Cells[sym_x][sym_y]->GetP(i)/Cells[sym_x][sym_y]->GetMolMass();

								u_y += Cells[sym_x][sym_y]->real_SPEED_CUBE[i][j][k]*
									Cells[sym_x][sym_y]->GetP(j)/Cells[sym_x][sym_y]->GetMolMass();

								u_z += Cells[sym_x][sym_y]->real_SPEED_CUBE[i][j][k]*
									Cells[sym_x][sym_y]->GetP(k)/Cells[sym_x][sym_y]->GetMolMass();
							}
						}
					}
				}
				u_x /= n;
				u_y /= n;
				u_z /= n;
				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							if( Cells[sym_x][sym_y]->GetP(i,j,k) > MainInfo_->p_cut &&
								MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
							}
							else {
								temperature += Cells[sym_x][sym_y]->GetMolMass() *
									( QUAD(Cells[sym_x][sym_y]->GetP(i)/Cells[sym_x][sym_y]->GetMolMass()-u_x)+
									QUAD(Cells[sym_x][sym_y]->GetP(j)/Cells[sym_x][sym_y]->GetMolMass()-u_y)+
									QUAD(Cells[sym_x][sym_y]->GetP(k)/Cells[sym_x][sym_y]->GetMolMass()-u_z) )*
									Cells[sym_x][sym_y]->real_SPEED_CUBE[i][j][k];			
							}
						}
					}
				}
				temperature /= n;
				// �����������
				temperature /= 3;

				fprintf(file_temperature, "%i %i %f\n", x, y, temperature);
				if(temperature > local_max_temperature)
					local_max_temperature = temperature;
				if(temperature < local_min_temperature)
					local_min_temperature = temperature;
			}
    }    
	}
  if(local_max_temperature > max_temperature)
    max_temperature = local_max_temperature;
  if(local_min_temperature < min_temperature)
    min_temperature = local_min_temperature;
  fclose(file_temperature);

  char file_name2[50];
  sprintf(file_name2, "data/output/gas %i/max_and_min_conc_and_temp%i.txt", gas_index, number_of_file);  
  file_info = fopen( file_name2, "a" );
  fprintf( file_info, "%f\n", local_max_temperature );
  fprintf( file_info, "%f\n", local_min_temperature );
  fclose( file_info );
  return;
}

void OutMacroParameters::StreamOut(void) {
  char file_name[50];
  sprintf(file_name, "data/output/gas %i/stream_of_heap%i.txt", gas_index, number_of_file);  
  file_stream_of_heap = fopen(file_name, "w");
  //file_stream_of_heap = fopen("stream_of_heap.txt", "w");
  double u_x; double u_y;
  double q_x; double q_y;
	double mod_gas_velocity;
  for(int y=0; y<Grid_->size_y; y++) {
    for(int x=0; x<Grid_->size_x; x++) {
			if( !Cells[x][y] || Cells[x][y]->type == 9 ) {
				fprintf(file_stream_of_heap, "%i %i %f %f\n", x, y, 100.0f, 100.0f);
			}
			else {
				u_x = 0.0f;
				u_y = 0.0f;
				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							if( Cells[x][y]->GetP(i,j,k) > MainInfo_->p_cut &&
								MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
							}
							else {
								u_x += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
									Cells[x][y]->GetP(i)/Cells[x][y]->GetMolMass()*CUBE(MainInfo_->delta_p/Cells[x][y]->GetMolMass());
								u_y += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
									Cells[x][y]->GetP(j)/Cells[x][y]->GetMolMass()*CUBE(MainInfo_->delta_p/Cells[x][y]->GetMolMass());
							}
						}
					}
				}
				q_x = 0.0f; q_y = 0.0f;
				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							if( Cells[x][y]->GetP(i,j,k) > MainInfo_->p_cut &&
								MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
							}
							else {
								q_x += Cells[x][y]->GetP(i)/Cells[x][y]->GetMolMass()*QUAD(Cells[x][y]->GetP(i)/Cells[x][y]->GetMolMass()-u_x)*
									Cells[x][y]->real_SPEED_CUBE[i][j][k]*MainInfo_->delta_p/Cells[x][y]->GetMolMass();
								q_y += Cells[x][y]->GetP(j)/Cells[x][y]->GetMolMass()*QUAD(Cells[x][y]->GetP(j)/Cells[x][y]->GetMolMass()-u_y)*
									Cells[x][y]->real_SPEED_CUBE[i][j][k]*MainInfo_->delta_p/Cells[x][y]->GetMolMass();
							}
						}
					}
				}
				//q_x *= Cells[x][y]->GetMolMass() / 2.0f;
				//q_y *= Cells[x][y]->GetMolMass() / 2.0f;
				mod_gas_velocity = sqrt( q_x*q_x + q_y*q_y );
				if( mod_gas_velocity > max_mod_gas_velocity )
					max_mod_gas_velocity = mod_gas_velocity;
				if( mod_gas_velocity < min_mod_gas_velocity )
					min_mod_gas_velocity = mod_gas_velocity;
				fprintf(file_stream_of_heap, "%i %i %f %f\n", x, y, q_x, q_y);
			}
    }    
	}
  fclose(file_stream_of_heap);
  return;
}

void OutMacroParameters::VelocityOut(void) {
  char file_name[50];
  sprintf(file_name, "data/output/gas %i/gas_velocity%i.txt", gas_index, number_of_file);
  gas_velocity = fopen(file_name, "w");
  double u_x; double u_y;
	double mod_gas_velocity;
	double n;
  for(int y=0; y<Grid_->size_y; y++) {
    for(int x=0; x<Grid_->size_x; x++) {
			if( !Cells[x][y] || Cells[x][y]->type == 9 ) {
				fprintf(gas_velocity, "%i %i %f %f\n", x, y, 100.0f, 100.0f);
			}
			else {
				u_x = 0.0f;
				u_y = 0.0f;
				n = 0.0f;
				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							if( Cells[x][y]->GetP(i,j,k) > MainInfo_->p_cut &&
								MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
							}
							else {
								n += Cells[x][y]->real_SPEED_CUBE[i][j][k];
								// ����� ��� ���� �������� ������ ����, � �� ��� ��� ������� �����-��
								// ��� ������ �����
								// ������ ��  � �������! ���� � �������� �,
								// �������������, ������ ���� � ��������� ���(����� � � 10) ����� �������!!!
								// �, ����, ��� ����
								// ��� � ������ ���� ������������� �� ��������� �����
								// � ���� � ������� ������ ������ ������ ��������
								// ���� ��� � �������!
								u_x += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
								Cells[x][y]->GetP(i)/Cells[x][y]->GetMolMass();
								u_y += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
								Cells[x][y]->GetP(j)/Cells[x][y]->GetMolMass();
							}
						}
					}
				}
				u_x /= n;
				u_y /= n;
				mod_gas_velocity = sqrt( u_x*u_x + u_y*u_y );
				if( mod_gas_velocity > max_mod_gas_velocity )
					max_mod_gas_velocity = mod_gas_velocity;
				if( mod_gas_velocity < min_mod_gas_velocity )
					min_mod_gas_velocity = mod_gas_velocity;
				fprintf(gas_velocity, "%i %i %f %f\n", x, y, u_x, u_y);
			}
    }    
	}
  fclose(gas_velocity);
  return;
}

void OutMacroParameters::ConsuptionOut(void) {
	for( int i=0; i<cons_areas_quantity; i++ )
		ConsuptionAreaOut( consuptions_areas[i], i );
  return;
}

void OutMacroParameters::InfoOut(clock_t start, int main_circle) {
  printf("\rThe output of intermediate results. It may take a few minutes...");
  char file_name[50];
  sprintf(file_name, "data/output/gas %i/info%i.txt", gas_index, number_of_file);
  file_info = fopen(file_name, "w");
  if(file_info == NULL) {
    printf("Please, create folder 'Data'.\n");
    return;
  }
  //file_info = fopen("info.txt", "w");
	int procent;
	double workTime;
	procent = (int)(( (float)main_circle )*100.0f/( (float)MainInfo_->quantity_of_iteration ));
	fprintf(file_info, "Complete: %i%%\n", procent);
	clock_t finish=clock();    
	workTime=(double)(clock() - start) / CLOCKS_PER_SEC;
	fprintf(file_info, "Time passed: %.0f seconds\n", workTime );
	fprintf(file_info, "Time left: %.0f seconds\n", workTime * ( 100.0f - procent ) / procent );
  fprintf(file_info, "Quantity of iteration now: %i\n", main_circle);
  fprintf(file_info, "Total number of iteration: %i\n", MainInfo_->quantity_of_iteration);
  fprintf(file_info, "Grid size x: %i\n", Grid_->size_x);
  fprintf(file_info, "Grid size y: %i\n", Grid_->size_y);
	fprintf(file_info, "Time step: %lf\n", MainInfo_->time_step);
  //fprintf(file_info, "Depth of the gap: %i\n", MainGrid_->depth_of_the_gap);
  //fprintf(file_info, "T1: %f\n", MainInfo_->T1);
  //fprintf(file_info, "Temperature of wall2: %f\n", Grid_->T_2_wall);
  fclose(file_info);
  return;
}

void OutMacroParameters::OutGlobalParameters(void) {
	number_of_file++;
	if( gas_index == 1 ) 
		file_global_info = fopen("data/output/gas 1/global_info.txt", "w");
	else
		file_global_info = fopen("data/output/gas 2/global_info.txt", "w");
  fprintf(file_global_info, "%f\n%f\n%f\n%f\n%f\n%f\n", min_concentration,
    max_concentration, min_temperature, max_temperature,
		min_mod_gas_velocity, max_mod_gas_velocity );
  fclose(file_global_info);
  return;
}

bool OutMacroParameters::FillConsuptionAreas( void ) {
	// ����� ����� ��� ����� ������������ ��� ����� � ����� (������� �������� ������� ����)
	MAIN_INFO* main_info = MainInfo_->main_info;
	consuptions_areas = new CONSUPTION_AREA*[200];
	for( int i = 0; i<main_info->gaps_quantity; i++ ) {
		// ������� (�����)
		consuptions_areas[2*i] = new CONSUPTION_AREA;
		consuptions_areas[2*i]->coord.x = main_info->gaps_start_x + (main_info->d+main_info->h)*i;
		consuptions_areas[2*i]->coord.y = main_info->gaps_start_y;
		consuptions_areas[2*i]->orientation = consuptions_areas[0]->HORISONTAL;
		consuptions_areas[2*i]->size = main_info->d;
		consuptions_areas[2*i]->name = "gap_" + mmmy_to_string((long double)i) + "_down";
		// ������� (������)
		consuptions_areas[2*i+1] = new CONSUPTION_AREA;
		consuptions_areas[2*i+1]->coord.x = main_info->gaps_start_x + (main_info->d+main_info->h)*i;
		consuptions_areas[2*i+1]->coord.y = main_info->gaps_start_y+(main_info->depth_of_the_gap-1);
		consuptions_areas[2*i+1]->orientation = consuptions_areas[0]->HORISONTAL;
		consuptions_areas[2*i+1]->size = main_info->d;
		consuptions_areas[2*i+1]->name = "gap_" + mmmy_to_string((long double)i) + "_up";
	}
	cons_areas_quantity = main_info->gaps_quantity*2;
	// ������ ���� ����������� � ������� � �������� �������
	int num_area = (main_info->gaps_quantity-1)*2+2;
	// ����� ��������
	consuptions_areas[num_area] = new CONSUPTION_AREA;
	consuptions_areas[num_area]->coord.x = main_info->sp_start_x+main_info->sp_size_x;
	consuptions_areas[num_area]->coord.y = main_info->sp_start_y+main_info->l_sp+main_info->depth_of_the_gap;
	consuptions_areas[num_area]->orientation = consuptions_areas[num_area]->VERTICAL;
	consuptions_areas[num_area]->size = main_info->l_sp-1;
	consuptions_areas[num_area]->name = "up_canal_out";
	num_area++;
	cons_areas_quantity++;
	// ���� ��������
	consuptions_areas[num_area] = new CONSUPTION_AREA;
	consuptions_areas[num_area]->coord.x = main_info->sp_start_x-1;
	consuptions_areas[num_area]->coord.y = main_info->sp_start_y+main_info->l_sp+main_info->depth_of_the_gap;
	consuptions_areas[num_area]->orientation = consuptions_areas[num_area]->VERTICAL;
	consuptions_areas[num_area]->size = main_info->l_sp-1;
	consuptions_areas[num_area]->name = "up_canal_in";
	num_area++;
	cons_areas_quantity++;
	// ����� �������
	consuptions_areas[num_area] = new CONSUPTION_AREA;
	consuptions_areas[num_area]->coord.x = main_info->sp_start_x+main_info->sp_size_x;
	consuptions_areas[num_area]->coord.y = main_info->sp_start_y+1;
	consuptions_areas[num_area]->orientation = consuptions_areas[num_area]->VERTICAL;
	consuptions_areas[num_area]->size = main_info->l_sp-1;
	consuptions_areas[num_area]->name = "down_canal_out";
	num_area++;
	cons_areas_quantity++;
	// ���� �������
	consuptions_areas[num_area] = new CONSUPTION_AREA;
	consuptions_areas[num_area]->coord.x = main_info->sp_start_x-1;
	consuptions_areas[num_area]->coord.y = main_info->sp_start_y+1;
	consuptions_areas[num_area]->orientation = consuptions_areas[num_area]->VERTICAL;
	consuptions_areas[num_area]->size = main_info->l_sp-1;
	consuptions_areas[num_area]->name = "down_canal_in";
	num_area++;
	cons_areas_quantity++;
	return true;
}

void OutMacroParameters::ConsuptionAreaOut( CONSUPTION_AREA* area, int num ) {
  char file_name[50];
	sprintf(file_name, "data/output/gas %i/cons_%s.txt", gas_index, area->name.data() );
	if( !gas_consuption[num] )
		gas_consuption[num] = fopen(file_name, "w");
	else
		gas_consuption[num] = fopen(file_name, "a");

  double u_x; double u_y;
	double consuption = 0;
	int x, y;
	for( int i=0; i<area->size; i++ ) {
		x = area->coord.x;
		y = area->coord.y;
		switch ( area->orientation  ) {
		case CONSUPTION_AREA::HORISONTAL:
			x += i;
			break;
		case CONSUPTION_AREA::VERTICAL:
			y += i;
			break;
		}
		if( !Cells[x][y] || Cells[x][y]->type == 9 ) {
			printf("Error of consuption area\n");
			return;
		}
		else {
			u_x = 0.0f;
			u_y = 0.0f;
			for(int i=0; i<MainInfo_->speed_quantity; i++) {
				for(int j=0; j<MainInfo_->speed_quantity; j++) {
					for(int k=0; k<MainInfo_->speed_quantity; k++) {
						if( Cells[x][y]->GetP(i,j,k) > MainInfo_->p_cut &&
							MainInfo_->FLAG_use_cut_sphere ) {
							// do nothing
						}
						else {
							//Cells[x][y]->real_SPEED_CUBE[i][j][k] = 0;
							u_x += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
								Cells[x][y]->GetP(i)/Cells[x][y]->GetMolMass();
							u_y += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
								Cells[x][y]->GetP(j)/Cells[x][y]->GetMolMass();
						}
					}
				}
			}
			switch ( area->orientation  ) {
			case CONSUPTION_AREA::HORISONTAL:
				consuption += u_y;
				break;
			case CONSUPTION_AREA::VERTICAL:
				consuption += u_x;
				break;
			}			
		}
	}

	fprintf( gas_consuption[num], "%f\n", consuption );

  fclose( gas_consuption[num] );
	return;
}
