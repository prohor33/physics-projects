#include"cell.h"
#include"main_info.h"

Cell::Cell( int speed_quantity ) {

  real_SPEED_CUBE = new double**[speed_quantity];
	double* a;
	int start;
	int length;
  for(int i=0; i<speed_quantity; i++) {
    real_SPEED_CUBE[i] = new double*[speed_quantity];
    for(int j=0; j<speed_quantity; j++) {
			if( MainInfo_->FLAG_use_only_sphere_data ) {
				start = -1;
				length = 0;
				for(int k=0; k<speed_quantity; k++) {
					if( GetP(i,j,k) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {	
							// do nothing
					}
					else {
						if( start == -1 )
							start = k;
						length++;
					}			
				}
				real_SPEED_CUBE[i][j] = new double[length];
				real_SPEED_CUBE[i][j] -= start;

				//for(int k=0; k<speed_quantity; k++) {
				//	if( GetP(i,j,k) > MainInfo_->p_cut &&
				//		MainInfo_->FLAG_use_cut_sphere ) {
				//			// do nothing
				//	}
				//	else {
				//		// ��������
				//		//real_SPEED_CUBE[i][j][k] = 1.23f;
				//		//printf("%f\n", real_SPEED_CUBE[i][j][k]);
				//	}
				//}
			}
			else {
				real_SPEED_CUBE[i][j] = new double[speed_quantity];					
			}
    }
  }
  future_SPEED_CUBE = real_SPEED_CUBE;  // it's the same

  SPEED_CUBE_half = new double**[speed_quantity];
  for(int i=0; i<speed_quantity; i++) {
    SPEED_CUBE_half[i] = new double*[speed_quantity];
    for(int j=0; j<speed_quantity; j++) {
			if( MainInfo_->FLAG_use_only_sphere_data ) {
				start = -1;
				length = 0;
				for(int k=0; k<speed_quantity; k++) {
					if( GetP(i,j,k) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {	
							// do nothing
					}
					else {
						if( start == -1 )
							start = k;
						length++;
					}			
				}
				SPEED_CUBE_half[i][j] = new double[length];
				SPEED_CUBE_half[i][j] -= start;

				//for(int k=0; k<speed_quantity; k++) {
				//	if( GetP(i,j,k) > MainInfo_->p_cut &&
				//		MainInfo_->FLAG_use_cut_sphere ) {
				//			// do nothing
				//	}
				//	else {
				//		// ��������
				//		SPEED_CUBE_half[i][j][k] = 0.0f;
				//	}
				//}
			}
			else {
				SPEED_CUBE_half[i][j] = new double[speed_quantity];					
			}   
    }
  }

  Cell_x_plus = NULL;
  Cell_x_minus = NULL;
  Cell_y_plus = NULL;
  Cell_y_minus = NULL;
  return;
}

double Cell::GetMolMass( void ) {
	switch( gas_n ) {
	case 1:
		return MainInfo_->mol_mass_1; 
	case 2:
		return MainInfo_->mol_mass_2;
	default:
		printf("Error of getting mol mass\n");
		break;
	}
	return 0;
}

double Cell::GetP( int i, ... ) {
	return MainInfo_->p[i];
}
double Cell::GetP( int i, int j, int k, ... ) {
	return sqrt( MainInfo_->p[i]*MainInfo_->p[i] + MainInfo_->p[j]*MainInfo_->p[j] + MainInfo_->p[k]*MainInfo_->p[k] );
}
double Cell::GetP2( int i, int j, int k, ... ) {
	return ( MainInfo_->p[i]*MainInfo_->p[i] + MainInfo_->p[j]*MainInfo_->p[j] + MainInfo_->p[k]*MainInfo_->p[k] );
}

// 1. ������� ����� ��������� ��� ���������� ��������
// ���! �� ���� ��� ������� ������ �� ������ �����
// ���� ������ CountHalfCell
// ��� ����� ����� ����� ����� � ����� ����������� �������
// ����� - ��� ������ ������ �������� ������ �� ������������ ��������
// �� ���� �� 4.8f
// MC ������� ��� �� ����� ������������� ����� �������� ������       

// 2. ����� �� ������ ���������� �������� ��������� ��������� �������
// ��� ����� ����� ����� tau � h
// ������ ����� ���� ������ �������� ����� �����, ��� � "���������"
// �� ����

// ��������! ������� ������ ����������� �� ���!

OrdinaryCell::OrdinaryCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}

double Cell::GetLimiter_x( int x, int y, int z ) {
  if( !Cell_x_plus || !Cell_x_minus ) {
    printf("Error: corruption of the grid. Use Cell_x_plus or Cell_x_minus = NULL Cell(%i, %i)\n",
			coord_x, coord_y	);
    return -1.0f;
  }
  double val = real_SPEED_CUBE[x][y][z];
  double next = Cell_x_plus->real_SPEED_CUBE[x][y][z];
  double prev = Cell_x_minus->real_SPEED_CUBE[x][y][z];
  switch( MainInfo_->FLAG_limiter ) {
  case 1:  // superbee
    return max_( min_( 2.0f*module( next - val ), module( val - prev ) ), min_( module( next - val ),
      2.0f*module( val - prev ) ) );
    break;
  case 2:  // MC
    return min_( 
      min_( module( next - prev ) / 2.0f, 2.0f * module( next - val ) ), 2.0f * module( val - prev ) ) *
      sign( next - val );
    break;
  case 3:  // strange
    return max_( 0.0f, min_( 2.0f*module( val - prev ), min_( module( next - val ),
      min_( module( val - prev ), 2.0f*module( next - val ) ) ) ) ) * sign( next - val );
    break;
  default:
    printf( "Error limiter\n" );		
    return 0.0f;
    break;
  }  
}

double Cell::GetLimiter_y( int x, int y, int z ) {  
  if( !Cell_y_plus || !Cell_y_minus ) {
    printf("Error: corruption of the grid. Use Cell_x_plus or Cell_x_minus = NULL Cell(%i, %i)\n",
			coord_x, coord_y	);
    return -1.0f;
  }
  double val = real_SPEED_CUBE[x][y][z];
  double next = Cell_y_plus->real_SPEED_CUBE[x][y][z];
  double prev = Cell_y_minus->real_SPEED_CUBE[x][y][z];
  switch( MainInfo_->FLAG_limiter ) {
  case 1:  // superbee
    return max_( min_( 2.0f*module( next - val ), module( val - prev ) ), min_( module( next - val ),
      2.0f*module( val - prev ) ) );
    break;
  case 2:  // MC
    return min_( 
      min_( module( next - prev ) / 2.0f, 2.0f * module( next - val ) ), 2.0f * module( val - prev ) ) *
      sign( next - val );
    break;
  case 3:  // strange
    return max_( 0.0f, min_( 2.0f*module( val - prev ), min_( module( next - val ),
      min_( module( val - prev ), 2.0f*module( next - val ) ) ) ) ) * sign( next - val );
    break;
  default:
    printf( "Error limiter\n" );
    return 0.0f;
    break;
  }
}

void Cell::CountHalfCell_with_x_step( double like_a_tau ) {
  if( !Cell_x_plus ) {
    printf("Error: corruption of the grid. Use Cell_x_plus or Cell_x_minus = NULL Cell(%i, %i)\n",
			coord_x, coord_y	);
		return;
  }
  double gamma;  
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
      for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
          for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
						if( GetP(x,y,z) > MainInfo_->p_cut &&
							MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
						}
						else {
							//gamma = like_a_tau * MainInfo_->speed_values[x]/4.8f;
							gamma = like_a_tau * GetP(x) / GetMolMass() * MainInfo_->time_step / h_x;
							if( GetP(x) > 0.0f ) {
								// for speed > 0
								SPEED_CUBE_half[x][y][z] = real_SPEED_CUBE[x][y][z] +
									( 1.0f - fabs( gamma ) ) / 2.0f * GetLimiter_x( x, y, z );
							}
							else {
								// for speed < 0
								SPEED_CUBE_half[x][y][z] = Cell_x_plus->real_SPEED_CUBE[x][y][z] -
									( 1.0f - fabs( gamma ) ) / 2.0f * Cell_x_plus->GetLimiter_x( x, y, z );
							}
						}
          }
      }
  }
  return;
}

void Cell::CountHalfCell_with_y_step( double like_a_tau ) {
  if( !Cell_y_plus ) {
    printf("Error: corruption of the grid. Use Cell_x_plus or Cell_x_minus = NULL Cell(%i, %i)\n",
			coord_x, coord_y	);
    return;
  }
  double gamma;  
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
      for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
          for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
						if( GetP(x,y,z) > MainInfo_->p_cut &&
							MainInfo_->FLAG_use_cut_sphere ) {
								// do nothing
						}
						else {
							//gamma = like_a_tau * MainInfo_->speed_values[y]/4.8f;
							gamma = like_a_tau * GetP(y) / GetMolMass() * MainInfo_->time_step / h_y;
							if( GetP(y) > 0.0f ) {
								// for speed > 0
								SPEED_CUBE_half[x][y][z] = real_SPEED_CUBE[x][y][z] +
									( 1.0f - fabs( gamma ) ) / 2.0f * GetLimiter_y( x, y, z );
							}
							else {
								// for speed < 0
								SPEED_CUBE_half[x][y][z] = Cell_y_plus->real_SPEED_CUBE[x][y][z] -
									( 1.0f - fabs( gamma ) ) / 2.0f * Cell_y_plus->GetLimiter_y( x, y, z );
							}
						}
          }
      }
  }
  return;
}

void Cell::CountCell_with_x_step( double like_a_tau ) {
  if( !SPEED_CUBE_half || !Cell_x_minus || !Cell_x_minus->SPEED_CUBE_half ) {
    printf("Error: corruption");
  }
  double gamma;  
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
      for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
          for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
						if( GetP(x,y,z) > MainInfo_->p_cut &&
             MainInfo_->FLAG_use_cut_sphere ) {
              // do nothing               
            }
            else {
              //gamma = like_a_tau*MainInfo_->speed_values[x]/4.8f;
							gamma = like_a_tau * GetP(x) / GetMolMass() * MainInfo_->time_step / h_x;
              future_SPEED_CUBE[x][y][z] = real_SPEED_CUBE[x][y][z] -
                gamma * ( SPEED_CUBE_half[x][y][z] - Cell_x_minus->SPEED_CUBE_half[x][y][z] );
            }
          }
      }
  }
  return;
}

void Cell::CountCell_with_y_step( double like_a_tau ) {
  double gamma;  
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
      for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
          for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
						if( GetP(x,y,z) > MainInfo_->p_cut &&
             MainInfo_->FLAG_use_cut_sphere ) {
              // do nothing
            }
            else {             
							gamma = like_a_tau * GetP(y) / GetMolMass() * MainInfo_->time_step / h_y;
              future_SPEED_CUBE[x][y][z] = real_SPEED_CUBE[x][y][z] -
                gamma * ( SPEED_CUBE_half[x][y][z] - Cell_y_minus->SPEED_CUBE_half[x][y][z] );
            }
          }
      }
  }
  return;
}

double sign( double x ) {
  if( x > 0 )
    return 1.0f;
  if( x < 0 )
    return -1.0f;
  return 0.0f;
}

double module( double x ) {
  return x * sign( x );
}

double min_( double x, double y ) {
  if( x > y )
    return y;
  else
    return x;
}

double max_( double x, double y ) {
  if( x < y )
    return y;
  else
    return x;
}

//double square_of_speed( int x, int y, int z ) {
//  return ( MainInfo_->speed_values[x]*MainInfo_->speed_values[x] +
//            MainInfo_->speed_values[y]*MainInfo_->speed_values[y] +
//            MainInfo_->speed_values[z]*MainInfo_->speed_values[z] );
//}
//double mod_of_speed( int x, int y, int z ) {
//  return sqrt( MainInfo_->speed_values[x]*MainInfo_->speed_values[x] +
//            MainInfo_->speed_values[y]*MainInfo_->speed_values[y] +
//            MainInfo_->speed_values[z]*MainInfo_->speed_values[z] );
//}

LeftBorderCell::LeftBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void LeftBorderCell::CountHalfCell_with_x_step( double like_a_tau ) {
  CountHalfCell_with_x_step_LeftBorderCell( this , like_a_tau );
  return;
}

RightBorderCell::RightBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void RightBorderCell::CountHalfCell_with_x_step( double like_a_tau ) {
  CountHalfCell_with_x_step_RightBorderCell( this , like_a_tau );
  return;
}

UpBorderCell::UpBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void UpBorderCell::CountHalfCell_with_y_step( double like_a_tau ) {
  CountHalfCell_with_y_step_UpBorderCell( this, like_a_tau );
  return;
}

DownBorderCell::DownBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void DownBorderCell::CountHalfCell_with_y_step( double like_a_tau ) {
  CountHalfCell_with_y_step_DownBorderCell( this, like_a_tau );
  return;
}

UpLeftBorderCell::UpLeftBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void UpLeftBorderCell::CountHalfCell_with_y_step( double like_a_tau ) {
  CountHalfCell_with_y_step_UpBorderCell( this, like_a_tau );
  return;
}
void UpLeftBorderCell::CountHalfCell_with_x_step( double like_a_tau ) {
  CountHalfCell_with_x_step_LeftBorderCell( this, like_a_tau );
  return;
}

DownLeftBorderCell::DownLeftBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void DownLeftBorderCell::CountHalfCell_with_y_step( double like_a_tau ) {
  CountHalfCell_with_y_step_DownBorderCell( this, like_a_tau );
  return;
}
void DownLeftBorderCell::CountHalfCell_with_x_step( double like_a_tau ) {
  CountHalfCell_with_x_step_LeftBorderCell( this, like_a_tau );
  return;
}

UpRightBorderCell::UpRightBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void UpRightBorderCell::CountHalfCell_with_y_step( double like_a_tau ) {
  CountHalfCell_with_y_step_UpBorderCell( this, like_a_tau );
  return;
}
void UpRightBorderCell::CountHalfCell_with_x_step( double like_a_tau ) {
  CountHalfCell_with_x_step_RightBorderCell( this, like_a_tau );
  return;
}

DownRightBorderCell::DownRightBorderCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void DownRightBorderCell::CountHalfCell_with_y_step( double like_a_tau ) {
  CountHalfCell_with_y_step_DownBorderCell( this, like_a_tau );
  return;
}
void DownRightBorderCell::CountHalfCell_with_x_step( double like_a_tau ) {
  CountHalfCell_with_x_step_RightBorderCell( this, like_a_tau );
  return;
}



// --------------------------------------------------------------------------------------- x Left

void CountHalfCell_with_x_step_LeftBorderCell(Cell* c, double like_a_tau) {
  
  double numenator = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;

	// ��� ������������� ��������� � ������� (18.5) ������� Cell_x_minus
  double gamma; 
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {
					//gamma = like_a_tau * MainInfo_->speed_values[x]/4.8f;
					gamma = like_a_tau * c->GetP(x) / c->GetMolMass() * MainInfo_->time_step / c->h_x;
					if( c->GetP(x) > 0.0f ) {
						// for speed > 0         
						denominator += fabs( c->GetP(x) / c->GetMolMass() ) *
							exp( (-1.0f)*c->GetP2(x,y,z)/(2.0f*c->GetMolMass()*c->T_st_x) );
					}
					else {
						// for speed < 0							
						c->Cell_x_minus->real_SPEED_CUBE[x][y][z] = 
							max_( 0.0f, 2.0f*c->real_SPEED_CUBE[x][y][z] - c->Cell_x_plus->real_SPEED_CUBE[x][y][z] );

						c->Cell_x_minus->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] -
							( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_x( x, y, z );

						numenator += fabs( c->GetP(x) / c->GetMolMass() ) *
							c->Cell_x_minus->SPEED_CUBE_half[x][y][z];

						numenator2 += fabs( c->GetP(x) / c->GetMolMass() ) *
								0.5f *( c->Cell_x_minus->real_SPEED_CUBE[x][y][z] + c->real_SPEED_CUBE[x][y][z] );
					}
        }
			}
    }
  }

  // ��� ������������� �� ��������� ����������� Cell_x_minus->Half (f1/2)

  // Cell_x_minus->SPEED_CUBE_half ( f1/2 ) ��� ������������� �������� ����������� � (20.5)
  // ����� ���� ��������� Cell_x_minus->SPEED_CUBE_half ( f1/2 ) ��� ������������� ��������
  // ��������� ���������

  // ����� ����������� (20.6) � (20.5) ������ f1/2 ��� �������� ������ ����
  // ��������� g ��� �������� ������ ����.

  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {
					//gamma = like_a_tau * MainInfo_->speed_values[x]/4.8f;
					gamma = like_a_tau * c->GetP(x) / c->GetMolMass() * MainInfo_->time_step / c->h_x;
					if( c->GetP(x) > 0.0f ) {
						// for speed > 0
						 c->Cell_x_minus->SPEED_CUBE_half[x][y][z] = 
							 numenator / denominator * exp( (-1.0f)*c->GetP2(x,y,z)/(2.0f*c->GetMolMass()*c->T_st_x) );

						MainInfo_->g_speed[x][y][z] /* ��� g ���� ����������!!! */ = 
							numenator2 / denominator * exp( (-1.0f)*c->GetP2(x,y,z)/(2.0f*c->GetMolMass()*c->T_st_x) );

						c->Cell_x_minus->real_SPEED_CUBE[x][y][z] =
							max_( 0.0f, 2.0f*MainInfo_->g_speed[x][y][z] /* ��� g! */ - 
								c->real_SPEED_CUBE[x][y][z] );

						c->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] +
							( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_x( x, y, z );
					}
					else {
						// for speed < 0
						c->SPEED_CUBE_half[x][y][z] = c->Cell_x_plus->real_SPEED_CUBE[x][y][z] -
							( 1.0f - fabs( gamma ) ) / 2.0f * c->Cell_x_plus->GetLimiter_x( x, y, z );
					}
				}
      }
    }
  }

  // ���������� ����� g ��� �������� ������ ���� � (18.3), ������ �
  // Cell_x_minus->real_SPEED_CUBE ��� �������� ������� ����
  
  // ��� ��������� ��������� SPEED_CUBE_half (f3/2) ��� �������� ������ ����
  // � ��� �������� ������ ���� �� ����� ��� ������� � ������ :)
      
  // That's it =)  
  return;
}


// -----------------------------------------------------------------------------------------  x Right

void CountHalfCell_with_x_step_RightBorderCell( Cell* c, double like_a_tau ) {
// ��� ������������� ��������� � ������� (18.2) ������� Cell_x_plus

  double gamma; 
  double numenator = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {
					gamma = like_a_tau * c->GetP(x) / c->GetMolMass() * MainInfo_->time_step / c->h_x;
					if( c->GetP(x) > 0.0f ) {
						// for speed > 0              
						c->Cell_x_plus->real_SPEED_CUBE[x][y][z] = 
							max_( 0.0f, 2.0f*c->real_SPEED_CUBE[x][y][z] - c->Cell_x_minus->real_SPEED_CUBE[x][y][z] );

						c->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] +
							( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_x( x, y, z );

						numenator += fabs( c->GetP(x) / c->GetMolMass() ) *
							c->SPEED_CUBE_half[x][y][z];

						numenator2 += fabs( c->GetP(x) / c->GetMolMass() ) *
							0.5f *( c->real_SPEED_CUBE[x][y][z] + c->Cell_x_plus->real_SPEED_CUBE[x][y][z] );
					}
					else {
						// for speed < 0					
						denominator += fabs( c->GetP(x) / c->GetMolMass() ) *
							exp( (-1.0f)*c->GetP2(x,y,z)/(2.0f*c->GetMolMass()*c->T_st_x) );
					}
				}
      }
    }
  }

  // ��� ������������� �� ��������� ����������� Cell->Half (f N+1/2)

  // Cell->SPEED_CUBE_half ( f N+1/2 ) ��� ������������� �������� ����������� � (20.5)
  // ����� ���� ��������� Cell->SPEED_CUBE_half ( f N+1/2 ) ��� ������������� ��������
  // ��������� ���������

  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {
					//gamma = like_a_tau * MainInfo_->speed_values[x]/4.8f;
					gamma = like_a_tau * c->GetP(x) / c->GetMolMass() * MainInfo_->time_step / c->h_x;
					if( c->GetP(x) > 0.0f ) {
						// for speed > 0
						c->Cell_x_minus->SPEED_CUBE_half[x][y][z] = c->Cell_x_minus->real_SPEED_CUBE[x][y][z] +
							( 1.0f - fabs( gamma ) ) / 2.0f * c->Cell_x_minus->GetLimiter_x( x, y, z );
					}
					else {
						// for speed < 0
						 c->SPEED_CUBE_half[x][y][z] = 
							 numenator / denominator * exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_x) );

						 MainInfo_->g_speed[x][y][z] /* ��� g ���� ���������� � f0!!! */ = 
							 numenator2 / denominator *
							 exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_x) );

						 c->Cell_x_plus->real_SPEED_CUBE[x][y][z] =
							 max_( 0.0f, 2.0f*MainInfo_->g_speed[x][y][z] /* ��� g! */ - 
							 c->real_SPEED_CUBE[x][y][z] );

						 c->Cell_x_minus->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] -
							 ( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_x( x, y, z );
					}
				}
      }
    }
  }

  // ����� ����������� (20.6) � (20.5) ������ f N+1/2 ��� �������� ������ ����
  // ��������� g ��� �������� ������ ����.

  // ����� ���������� g �� f N+1->future_SPEED_CUBE !!!

  // ���������� ����� g ��� �������� ������ ���� � (18.6), ������ �
  // Cell_x_plus->real_SPEED_CUBE ��� �������� ������ ����
  
  // ��� ��������� ��������� Cell_x_minus->SPEED_CUBE_half (f N-1/2) ��� �������� ������ ����
  // � ��� �������� ������ ���� �� ����� ��� ������� � ������ :)
    
  // That's it =)
  return;
}

// ------------------------------------------------------------------------------------------ y Down ( 0 )

void CountHalfCell_with_y_step_DownBorderCell( Cell* c, double like_a_tau ) {

	// ��� ������������� ��������� � ������� (18.5) ������� Cell_x_minus

  double gamma;  
  double numenator = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {            
					//gamma = like_a_tau * MainInfo_->speed_values[y]/4.8f;
					gamma = like_a_tau * c->GetP(y) / c->GetMolMass() * MainInfo_->time_step / c->h_y;
					if( c->GetP(y) > 0.0f ) {
						// for speed > 0              
						denominator += fabs( c->GetP(y) / c->GetMolMass() ) * exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_y) );
					}
					else {
						// for speed < 0
						// real speed cube ���� ��������� � �������� ������?
						c->Cell_y_minus->real_SPEED_CUBE[x][y][z] = 
							max_( 0.0f, 2.0f*c->real_SPEED_CUBE[x][y][z] - c->Cell_y_plus->real_SPEED_CUBE[x][y][z] );
						c->Cell_y_minus->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] -
							( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_y( x, y, z );
						numenator += fabs( c->GetP(y) / c->GetMolMass() ) *
							c->Cell_y_minus->SPEED_CUBE_half[x][y][z];
						numenator2 += fabs( c->GetP(y) / c->GetMolMass() ) *
							0.5f *( c->Cell_y_minus->real_SPEED_CUBE[x][y][z] + c->real_SPEED_CUBE[x][y][z] );    
					}
				}
      }
    }
  }

  // ��� ������������� �� ��������� ����������� Cell_x_minus->Half (f1/2)

  // Cell_x_minus->SPEED_CUBE_half ( f1/2 ) ��� ������������� �������� ����������� � (20.5)
  // ����� ���� ��������� Cell_x_minus->SPEED_CUBE_half ( f1/2 ) ��� ������������� ��������
  // ��������� ���������

  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {
					//gamma = like_a_tau * MainInfo_->speed_values[y]/4.8f;
					gamma = like_a_tau * c->GetP(y) / c->GetMolMass() * MainInfo_->time_step / c->h_y;
					if( c->GetP(y) > 0.0f ) {
						// for speed > 0
						 c->Cell_y_minus->SPEED_CUBE_half[x][y][z] = 
							 numenator / denominator * exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_y) );

						 MainInfo_->g_speed[x][y][z] /* ��� g ���� ���������� � f0!!! */ = 
							 numenator2 / denominator *
							 exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_y) );

						 c->Cell_y_minus->real_SPEED_CUBE[x][y][z] =
							 max_( 0.0f, 2.0f*MainInfo_->g_speed[x][y][z] /* ��� g! */ - 
							 c->real_SPEED_CUBE[x][y][z] );

						 c->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] +
							 ( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_y( x, y, z );
					}
					else {
						// for speed < 0
						c->SPEED_CUBE_half[x][y][z] = c->Cell_y_plus->real_SPEED_CUBE[x][y][z] -
							( 1.0f - fabs( gamma ) ) / 2.0f * c->Cell_y_plus->GetLimiter_y( x, y, z );
					}
				}
      }
    }
  }

  // ����� ����������� (20.6) � (20.5) ������ f1/2 ��� �������� ������ ����
  // ��������� g ��� �������� ������ ����.

  // ����� ���������� g �� f0->future_SPEED_CUBE !!!

  // ���������� ����� g ��� �������� ������ ���� � (18.3), ������ �
  // Cell_x_minus->real_SPEED_CUBE ��� �������� ������� ����
  
  // ��� ��������� ��������� SPEED_CUBE_half (f3/2) ��� �������� ������ ����
  // � ��� �������� ������ ���� �� ����� ��� ������� � ������ :)

  // That's it =)  
  return;
}


// ------------------------------------------------------------------------------------------ y Up ( N )

void CountHalfCell_with_y_step_UpBorderCell( Cell* c, double like_a_tau ) {
// ��� ������������� ��������� � ������� (18.2) ������� Cell_x_plus

  double gamma;  
  double numenator = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;
  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {
					//gamma = like_a_tau * MainInfo_->speed_values[y]/4.8f;
					gamma = like_a_tau * c->GetP(y) / c->GetMolMass() * MainInfo_->time_step / c->h_y;
					if( c->GetP(y) > 0.0f ) {
						// for speed > 0              
						c->Cell_y_plus->real_SPEED_CUBE[x][y][z] = 
							max_( 0.0f, 2.0f*c->real_SPEED_CUBE[x][y][z] - c->Cell_y_minus->real_SPEED_CUBE[x][y][z] );

						c->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] +
							( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_y( x, y, z );

						numenator += fabs( c->GetP(y) / c->GetMolMass() ) *
							c->SPEED_CUBE_half[x][y][z];

						numenator2 += fabs( c->GetP(y) / c->GetMolMass() ) *
							0.5f *( c->real_SPEED_CUBE[x][y][z] + c->Cell_y_plus->real_SPEED_CUBE[x][y][z] );
					}
					else {
						// for speed < 0			
						denominator += fabs( c->GetP(y) / c->GetMolMass() ) *
							exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_y) );
					}
				}
      }
    }
  }

  // ��� ������������� �� ��������� ����������� Cell->Half (f N+1/2)

  // Cell->SPEED_CUBE_half ( f N+1/2 ) ��� ������������� �������� ����������� � (20.5)
  // ����� ���� ��������� Cell->SPEED_CUBE_half ( f N+1/2 ) ��� ������������� ��������
  // ��������� ���������

  for( int x=0; x<MainInfo_->speed_quantity; x++ ) {
    for( int y=0; y<MainInfo_->speed_quantity; y++ ) {
      for( int z=0; z<MainInfo_->speed_quantity; z++ ) {
				if( c->GetP(x,y,z) > MainInfo_->p_cut &&
						MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
        }
				else {
					gamma = like_a_tau * c->GetP(y) / c->GetMolMass() * MainInfo_->time_step / c->h_y;
					if( c->GetP(y) > 0.0f ) {
						// for speed > 0
						c->Cell_y_minus->SPEED_CUBE_half[x][y][z] = c->Cell_y_minus->real_SPEED_CUBE[x][y][z] +
							( 1.0f - fabs( gamma ) ) / 2.0f * c->Cell_y_minus->GetLimiter_y( x, y, z );
					}
					else {
						// for speed < 0
						 c->SPEED_CUBE_half[x][y][z] = 
							 numenator / denominator * exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_y) );

						 MainInfo_->g_speed[x][y][z] /* ��� g ���� ���������� � f0!!! */ = 
							 numenator / denominator * exp( (-1.0f*c->GetP2(x,y,z))/(2.0f*c->GetMolMass()*c->T_st_y) );

						 c->Cell_y_plus->real_SPEED_CUBE[x][y][z] =
							 max_( 0.0f, 2.0f*MainInfo_->g_speed[x][y][z] /* ��� g! */ - 
							 c->real_SPEED_CUBE[x][y][z] );

						 c->Cell_y_minus->SPEED_CUBE_half[x][y][z] = c->real_SPEED_CUBE[x][y][z] -
							 ( 1.0f - fabs( gamma ) ) / 2.0f * c->GetLimiter_y( x, y, z );
					}		
				}
      }
    }
  }

  // ����� ����������� (20.6) � (20.5) ������ f N+1/2 ��� �������� ������ ����
  // ��������� g ��� �������� ������ ����.

  // ����� ���������� g �� f N+1->future_SPEED_CUBE !!!

  // ���������� ����� g ��� �������� ������ ���� � (18.6), ������ �
  // Cell_x_plus->real_SPEED_CUBE ��� �������� ������ ����
  
  // ��� ��������� ��������� Cell_x_minus->SPEED_CUBE_half (f N-1/2) ��� �������� ������ ����
  // � ��� �������� ������ ���� �� ����� ��� ������� � ������ :)
    
  // That's it =)
  return;
}

FakeCell::FakeCell( int speed_quantity ): Cell( speed_quantity ) {
  return;
}
void FakeCell::CountHalfCell_with_x_step( double like_a_tau ) {  
  return;
}
void FakeCell::CountHalfCell_with_y_step( double like_a_tau ) {  
  return;
}
void FakeCell::CountCell_with_x_step( double like_a_tau ) {  
  return;
}
void FakeCell::CountCell_with_y_step( double like_a_tau ) {  
  return;
}

