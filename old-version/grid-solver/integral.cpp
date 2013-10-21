#include"integral.h"

Integral* Integral_ = NULL;

Integral::Integral(void) {
	ci::init(&potential, ci::NO_SYMM);
	particle.d = 1.; // ������� ��������, ��� �������� ���� ����� 1. ��-�� ����������������?
	radius = MainInfo_->speed_quantity / 2.0f;
	a = MainInfo_->p_cut / radius;

  xyz2i = new int**[MainInfo_->speed_quantity];
  for( int i=0; i<MainInfo_->speed_quantity; i++ ) {
    xyz2i[i] = new int*[MainInfo_->speed_quantity];
      for( int j=0; j<MainInfo_->speed_quantity; j++ ) {
        xyz2i[i][j] = new int[MainInfo_->speed_quantity];
          for( int k=0; k<MainInfo_->speed_quantity; k++ ) {
            xyz2i[i][j][k] = i + 2*radius*(j + 2*radius*k);
          }
      }
  }

  input_for_ci1 = new double[ ( 2*radius )*( 2*radius )*( 2*radius ) ];
	input_for_ci2 = new double[ ( 2*radius )*( 2*radius )*( 2*radius ) ];
	return;
};


// f[i] - ������� ������������� � i-��� ������; f[i] - ����� ����? ����� � ���������� operator[](int j)
// ��������, ������� ������ ��������� double* f = new double[size];
// j - ������ � "������������" ������� �������������, �.�. ���������� �.�. �� �������� ���, ��������, ������� ��������� 
// double*** f1, � �������� ���, ��������, double *. ��� ��� ��� �������� (i,j,k) f[xyz2i[i][j][k]] ��������� �� f1[i][j][k].
// �.�. xyx2i[i][j][k] ��������������� ��� ������� �������� � ����.
//  ��������, ���� ���������� ����� - ���, �� xyz[i][j][k] = i + 2*radius*(j + 2*radius*k);

// ����� ����!!!  
void Integral::Iteration( void ) {
	if( !FLAG_use_ci )
		return;
	printf("\rcounting integral...                                           ");

	// 1<->1
	{
		Generate( MainInfo_->time_step, MainInfo_->mol_mass_1, MainInfo_->mol_mass_1, particle, particle );
		for( int x=0; x<Grid_->size_x; x++ ) {
			for( int y=0; y<Grid_->size_y; y++ ) {
				if( Grid_->Cells1[x][y] && Grid_->Cells1[x][y]->type != 9 ) { 
					FillInput( x, y );
					ci::iter( input_for_ci1, input_for_ci1 );
					GetInput( x, y );
				}
			}
		}
	}
	// 1<->2 & 2<->1
	{
		Generate( 2*MainInfo_->time_step, MainInfo_->mol_mass_1, MainInfo_->mol_mass_2, particle, particle );
		for( int x=0; x<Grid_->size_x; x++ ) {
			for( int y=0; y<Grid_->size_y; y++ ) {
				if( Grid_->Cells1[x][y] && Grid_->Cells1[x][y]->type != 9 ) { 
					FillInput( x, y );
					ci::iter( input_for_ci1, input_for_ci2 );
					GetInput( x, y );
				}
			}
		}
	}
	// 2<->2
	{
		Generate( MainInfo_->time_step, MainInfo_->mol_mass_2, MainInfo_->mol_mass_2, particle, particle );
		for( int x=0; x<Grid_->size_x; x++ ) {
			for( int y=0; y<Grid_->size_y; y++ ) {		
				if( Grid_->Cells1[x][y] && Grid_->Cells1[x][y]->type != 9 ) { 
					FillInput( x, y );
					ci::iter( input_for_ci2, input_for_ci2 );
					GetInput( x, y );
				}
			}
		}
	}
	return;
}

// ���������� ������������
void Integral::FillInput( int x, int y ) {
	for(int i=0; i<MainInfo_->speed_quantity; i++) {
		for(int j=0; j<MainInfo_->speed_quantity; j++) {
			for(int k=0; k<MainInfo_->speed_quantity; k++) {
				if( ( Grid_->Cells1[x][y]->GetP(i,j,k) > MainInfo_->p_cut ) &&
					MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
				}
				else {	
					input_for_ci1[xyz2i[i][j][k]] = Grid_->Cells1[x][y]->real_SPEED_CUBE[i][j][k];
					input_for_ci2[xyz2i[i][j][k]] = Grid_->Cells2[x][y]->real_SPEED_CUBE[i][j][k];
				}
			}
		}
	}
	return;
}

// �������� ������������
void Integral::GetInput( int x, int y ) {
	for(int i=0; i<MainInfo_->speed_quantity; i++) {
    for(int j=0; j<MainInfo_->speed_quantity; j++) {
      for(int k=0; k<MainInfo_->speed_quantity; k++) {
				if( ( Grid_->Cells1[x][y]->GetP(i,j,k) > MainInfo_->p_cut ) &&
					MainInfo_->FLAG_use_cut_sphere ) {
						// do nothing
				}
				else {
					Grid_->Cells1[x][y]->real_SPEED_CUBE[i][j][k] = input_for_ci1[xyz2i[i][j][k]];
					Grid_->Cells2[x][y]->real_SPEED_CUBE[i][j][k] = input_for_ci2[xyz2i[i][j][k]];
				}
      }
    }
  }
	return;
}

void Integral::Generate( double time_step, double m1, double m2,  ci::Particle p1, ci::Particle p2 ) {
	// ������� ����� � ��� ���������
	ci::gen( time_step, 50000, radius, radius,  // radius - ����� ����� ����� ������� � ���������� �����, �.�. 2*radius -- ����� ����� �� ��������
		xyz2i, xyz2i,                            // xyz2i - ��. ����?
			a, m1, m2,
			p1, p2);        // � - cut / radius, ��� cut - ������ �������� ������� �������������, ������ ����� 4.8
																							// 1. - ����� �������, ��� �������� ���� ����� 1. ��-�� ���������������
																							// 50000 - �������� ����� ��������		
	return;
}