#include"grid.h"
#include"main_info.h"
#include"edge_conditions.h"

Grid* Grid_ = NULL;
Symmetry* Symmetry_ = NULL;

// из битового файла
void Grid::NewGridFromByteFile( char* f_name,  int speed_quantity, Cell*** &Cells, int gas_n ) {
	
	this->FLAG_USE_SYMMETRY = NO_SYMMETRY;

	MAIN_INFO* main_info = MainInfo_->main_info;

	CELL* cell_struct = new CELL;

  char ch;
  FILE* fp = fopen( f_name, "rb" );

	if( !fread( &ch, sizeof(char), 1, fp ) ) {
		printf("error: can't read to file %s\n", f_name );
	}
	if( ch != START_MAIN_INFO ) {
		printf("error reading file\n");
		//return;
	}
	if( !fread( main_info, sizeof(MAIN_INFO), 1, fp ) ) {
		printf("error: can't read to file %s\n", f_name );
	}
	if( main_info->size_x < 0 ) {
		printf("error main_info->size_x < 0\n");
		//return;
	}
	fread( &ch, sizeof(char), 1, fp );
	if( ch != END_MAIN_INFO ) {
		printf("error reading file\n");
		//return;
	}
	//NewGrid( main_info->size_x, main_info->size_y );
	size_x = main_info->size_x;
	size_y = main_info->size_y;
  Cells = new Cell**[size_x];
  for( int x = 0; x<size_x; x++ ) {
    Cells[x] = new Cell*[size_y];
  }
	printf("reading grid size_x = %i, size_y = %i\n", main_info->size_x, main_info->size_y );

	// что-то вообще ничего не ясно =(
	for(int i=0; i<Grid_->size_x; i++) {
		for(int j=0; j<Grid_->size_y; j++) {
			fread( &ch, sizeof(char), 1, fp );
			if( ch != START_CELL ) {
				printf("error reading file\n");
				//return;
			}
			//Grid_->mass_pcells[i][j] = new Cell;

			// читает только coord_x и дальше фейлит, странно
			// читает и coord_y тоже иногда но полностью структуру считать не может
      if( !fread( cell_struct, sizeof(CELL), 1, fp ) ) {
				printf("error: can't read from file %s\n", f_name );
			}
			// один краевой слой фейковый, еще один спец классы, все остальное - обычные ячейки
			
			switch( cell_struct->type ) {
			case 0:
				Cells[i][j] = new OrdinaryCell( speed_quantity );        
				break;
			case 1:
				Cells[i][j] = new LeftBorderCell( speed_quantity );                
				break;    
			case 2:
				Cells[i][j] = new RightBorderCell( speed_quantity );        
				break;
			case 3:
				Cells[i][j] = new UpBorderCell( speed_quantity );        
				break;
			case 4:
				Cells[i][j] = new DownBorderCell( speed_quantity );        
				break;
			case 5:
				Cells[i][j] = new UpLeftBorderCell( speed_quantity );            
				break;
			case 6:
				Cells[i][j] = new DownLeftBorderCell( speed_quantity );              
				break;
			case 7:
				Cells[i][j] = new UpRightBorderCell( speed_quantity );        
				break;
			case 8:
				Cells[i][j] = new DownRightBorderCell( speed_quantity );        
				break;
			case 9:
				Cells[i][j] = new FakeCell( speed_quantity );        
				break;
			case -1:
        Cells[i][j] = NULL;        
        break;
			}   

			if( Cells[i][j] ) {
				Cells[i][j]->coord_x = i;
				Cells[i][j]->coord_y = j;
				Cells[i][j]->type = cell_struct->type;
				Cells[i][j]->T_start = cell_struct->T_start;
				Cells[i][j]->h_x = cell_struct->h_x;
				Cells[i][j]->h_y = cell_struct->h_y;
				Cells[i][j]->n = cell_struct->n; 
				Cells[i][j]->gas_n = gas_n;
			}
			
			if( !fread( &ch, sizeof(char), 1, fp ) ) {
				printf("error: can't read to file %s\n", f_name );
			}
			if( ch != END_CELL ) {
				printf("error reading file\n");
				//return;
			}
		}
  }

	fread( &ch, sizeof(char), 1, fp );
	if( ch != END_FILE ) {
		printf("error reading file\n");
		//return;
	}

	fclose( fp );

	// устанавливаем соседей
	for( int x = 0; x<size_x; x++ ) {
		for( int y = 0; y<size_y; y++ ) {  
			if( Cells[x][y] && Cells[x][y]->type != 9 ) {
				Cells[x][y]->Cell_x_minus = Cells[x-1][y];
				Cells[x][y]->Cell_x_plus = Cells[x+1][y];
				Cells[x][y]->Cell_y_minus = Cells[x][y-1];
				Cells[x][y]->Cell_y_plus = Cells[x][y+1];

				// устанавливаем температуру стенок
				if( Cells[x+1][y]->type == 9 )
					Cells[x][y]->T_st_x = Cells[x+1][y]->T_start;	
				if( Cells[x-1][y]->type == 9 )
					Cells[x][y]->T_st_x = Cells[x-1][y]->T_start;	
				if( Cells[x][y+1]->type == 9 )
					Cells[x][y]->T_st_y = Cells[x][y+1]->T_start;	
				if( Cells[x][y-1]->type == 9 )
					Cells[x][y]->T_st_y = Cells[x][y-1]->T_start;	
			}
		}
	}
  return;
}

void Grid::MakeStep( Cell*** Cells ) {
  // 0.5f x step пока что 1 тк половинные высчитываем тоже с 1
  //int board = 3;
	int gas_number = -1;
  int sym_size_x, sym_size_y;
  switch( FLAG_USE_SYMMETRY ) {
  case NO_SYMMETRY:
    sym_size_x = size_x;
    sym_size_y = size_y;
    break;
  case QUARTER_SYMMETRY:
    sym_size_x = size_x/2.0f;
    sym_size_y = size_y/2.0f;
    break;
  }
  int board = 0;
  double like_a_tau = 1.0f;
  for( int x = board; x<(sym_size_x-board); x++ ) {
    for( int y = board; y<(sym_size_y-board); y++ ) {
			if( Cells[x][y] ) {
				Cells[x][y]->CountHalfCell_with_x_step( like_a_tau / 2.0f );
				gas_number = Cells[x][y]->gas_n;
			}
    }
  }
  if( FLAG_USE_SYMMETRY != NO_SYMMETRY )
    Symmetry_->ReflectHalfGrid( Cells );
  for( int x = board; x<(sym_size_x-board); x++ ) {
    for( int y = board; y<(sym_size_y-board); y++ ) {
			if( Cells[x][y] )
				Cells[x][y]->CountCell_with_x_step( like_a_tau / 2.0f );      
    }
  }
  if( FLAG_USE_SYMMETRY != NO_SYMMETRY )
    Symmetry_->ReflectGrid( Cells );
  // 1.0f y step
  for( int x = board; x<(sym_size_x-board); x++ ) {
    for( int y = board; y<(sym_size_y-board); y++ ) {
			if( Cells[x][y] )
				Cells[x][y]->CountHalfCell_with_y_step( like_a_tau );
    }
  }
  if( FLAG_USE_SYMMETRY != NO_SYMMETRY )
    Symmetry_->ReflectHalfGrid( Cells );
  for( int x = board; x<(sym_size_x-board); x++ ) {
    for( int y = board; y<(sym_size_y-board); y++ ) {
			if( Cells[x][y] )
				Cells[x][y]->CountCell_with_y_step( like_a_tau );
    }
  }
  if( FLAG_USE_SYMMETRY != NO_SYMMETRY )
    Symmetry_->ReflectGrid( Cells );
  // 0.5f x step
  for( int x = board; x<(sym_size_x-board); x++ ) {
    for( int y = board; y<(sym_size_y-board); y++ ) {
			if( Cells[x][y] )
				Cells[x][y]->CountHalfCell_with_x_step( like_a_tau / 2.0f );
    }
  }
  if( FLAG_USE_SYMMETRY != NO_SYMMETRY )
    Symmetry_->ReflectHalfGrid( Cells );
  for( int x = board; x<(sym_size_x-board); x++ ) {
    for( int y = board; y<(sym_size_y-board); y++ ) {
			if( Cells[x][y] )
				Cells[x][y]->CountCell_with_x_step( like_a_tau / 2.0f );
    }
  }
  //if( FLAG_USE_SYMMETRY != NO_SYMMETRY )
  //  Symmetry_->ReflectGrid();   NONONO
	switch( gas_number ) {
	case 1:
		EdgeCondition1_->SetEdgeConditions();
		break;
	case 2:
		EdgeCondition2_->SetEdgeConditions();
		break;
	default:
		printf("Make step error\n");
		break;
	}
  return;
}

void Grid::SetInitialConditions( Cell*** Cells, ... ) {
	float C = 0.0f;
	double p_slash = 0;
	double k = MainInfo_->initial_stream;
	int x_sp;
	MAIN_INFO* main_info = MainInfo_->main_info;
	int area_size = main_info->l_sp-1;
  for(int x=0; x<Grid_->size_x; x++) {
    for(int y=0; y<Grid_->size_y; y++) {

			if( Cells[x][y] ) {
				C = 0.0f;
				// count C
				if( MainInfo_->FLAG_use_initial_stream ) {
					p_slash = 0;
					// верхний канал
					x_sp = y - ( main_info->sp_start_y+main_info->l_sp+main_info->depth_of_the_gap );
					if( 0 <= x_sp && x_sp < area_size )						
						p_slash = -k*x_sp*(x_sp-area_size-1)*Cells[x][y]->GetMolMass();

					// нижний канал
					x_sp = y - ( main_info->sp_start_y+1 );
					if( 0 <= x_sp && x_sp < area_size )						
						p_slash = -k*x_sp*(x_sp-area_size-1)*Cells[x][y]->GetMolMass();
				}
				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							C += exp((-1.0f)*
								( QUAD( Cells[x][y]->GetP(i) - p_slash ) + 
								QUAD( Cells[x][y]->GetP(j) ) +
								QUAD( Cells[x][y]->GetP(k) ) ) /
								(2.0f*Cells[x][y]->GetMolMass()*Cells[x][y]->T_start));
						}
					}
				}
				/* n0 = 1.0 */
				C = 1.0f / C;

				for(int i=0; i<MainInfo_->speed_quantity; i++) {
					for(int j=0; j<MainInfo_->speed_quantity; j++) {
						for(int k=0; k<MainInfo_->speed_quantity; k++) {
							if( ( Cells[x][y]->GetP(i,j,k) > MainInfo_->p_cut ) &&
							 MainInfo_->FLAG_use_cut_sphere ) {
								 // do nothing
							}
							else {					
								Cells[x][y]->real_SPEED_CUBE[i][j][k] = Cells[x][y]->n * C * exp((-1.0f)*
									( QUAD( Cells[x][y]->GetP(i) - p_slash ) + 
									QUAD( Cells[x][y]->GetP(j) ) +
									QUAD( Cells[x][y]->GetP(k) ) ) /
									(2.0f*Cells[x][y]->GetMolMass()*Cells[x][y]->T_start));
							}
						}
					}
				}
			}
    }
  }  
  return;
}

void Symmetry::ReflectHalfGrid( Cell*** Cells ) {
  int sym_size_x, sym_size_y;
  switch( Grid_->FLAG_USE_SYMMETRY ) {
  case NO_SYMMETRY:
    return;
    break;
  case QUARTER_SYMMETRY:
    sym_size_x = Grid_->size_x/2.0f;
    sym_size_y = Grid_->size_y/2.0f;
    for(int z=0; z<sym_size_x; z++) {
      for(int i=0; i<MainInfo_->speed_quantity; i++) {
        for(int j=0; j<MainInfo_->speed_quantity; j++) {
          for(int k=0; k<MainInfo_->speed_quantity; k++) {
            Cells[sym_size_x][z]->SPEED_CUBE_half[i][j][k] =
              Cells[sym_size_x-1][z]->SPEED_CUBE_half[i][j][k];
            Cells[sym_size_x+1][z]->SPEED_CUBE_half[i][j][k] =
              Cells[sym_size_x-2][z]->SPEED_CUBE_half[i][j][k];
            Cells[z][sym_size_y]->SPEED_CUBE_half[i][j][k] =
              Cells[z][sym_size_y-1]->SPEED_CUBE_half[i][j][k];
            Cells[z][sym_size_y+1]->SPEED_CUBE_half[i][j][k] =
              Cells[z][sym_size_y-2]->SPEED_CUBE_half[i][j][k];
          }
        }
      }
    }
    break;
  }
  return;
}

void Symmetry::ReflectGrid( Cell*** Cells ) {
  int sym_size_x, sym_size_y;
  switch( Grid_->FLAG_USE_SYMMETRY ) {
  case NO_SYMMETRY:
    return;
    break;
  case QUARTER_SYMMETRY:
    sym_size_x = Grid_->size_x/2.0f;
    sym_size_y = Grid_->size_y/2.0f;
    for(int z=0; z<sym_size_x; z++) {
      for(int i=0; i<MainInfo_->speed_quantity; i++) {
        for(int j=0; j<MainInfo_->speed_quantity; j++) {
          for(int k=0; k<MainInfo_->speed_quantity; k++) {
            Cells[sym_size_x][z]->real_SPEED_CUBE[i][j][k] =
              Cells[sym_size_x-1][z]->real_SPEED_CUBE[i][j][k];
            Cells[sym_size_x+1][z]->real_SPEED_CUBE[i][j][k] =
              Cells[sym_size_x-2][z]->real_SPEED_CUBE[i][j][k];
            Cells[z][sym_size_y]->real_SPEED_CUBE[i][j][k] =
              Cells[z][sym_size_y-1]->real_SPEED_CUBE[i][j][k];
            Cells[z][sym_size_y+1]->real_SPEED_CUBE[i][j][k] =
              Cells[z][sym_size_y-2]->real_SPEED_CUBE[i][j][k];
          }
        }
      }
    }
    break;
  }
  return;
}