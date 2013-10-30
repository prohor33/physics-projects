#include"edge_conditions.h"
#include"main_info.h"

EdgeCondition* EdgeCondition1_ = NULL;
EdgeCondition* EdgeCondition2_ = NULL;

bool EdgeCondition::FillEdgeConditionAreas( void ) {
	if( !MainInfo_->FLAG_use_edge_conditions )
		return false;
	MAIN_INFO* main_info = MainInfo_->main_info;
	// �� ����� ����� ������
	// �� ������� � ������
	edgecondition_areas = new EDGECONDITION_AREA*[4];
	edgecond_areas_quantity = 4;
	// ����� �� �������� �������� �� ����
	// ������ � ������ ������� (����� �������� ������)
	edgecondition_areas[0] = new EDGECONDITION_AREA;
	edgecondition_areas[0]->name = "up_canal_out";
	edgecondition_areas[0]->coord.x = main_info->size_x-2-edgecond_slash_from_edge;
	edgecondition_areas[0]->coord.y = main_info->sp_start_y+main_info->l_sp+main_info->depth_of_the_gap;
	edgecondition_areas[0]->orientation = edgecondition_areas[0]->VERTICAL;
	edgecondition_areas[0]->size = main_info->l_sp-1;
	edgecondition_areas[0]->start_consumption = -100.0f;

	// ����� �������
	edgecondition_areas[1] = new EDGECONDITION_AREA;
	edgecondition_areas[1]->name = "up_canal_in";
	edgecondition_areas[1]->coord.x = 1+edgecond_slash_from_edge;
	edgecondition_areas[1]->coord.y = main_info->sp_start_y+main_info->l_sp+main_info->depth_of_the_gap;
	edgecondition_areas[1]->orientation = edgecondition_areas[1]->VERTICAL;
	edgecondition_areas[1]->size = main_info->l_sp-1;
	edgecondition_areas[1]->start_consumption = -100.0f;

	// ������ ������
	edgecondition_areas[2] = new EDGECONDITION_AREA;
	edgecondition_areas[2]->name = "down_canal_out";
	edgecondition_areas[2]->coord.x = main_info->size_x-2-edgecond_slash_from_edge;
	edgecondition_areas[2]->coord.y = main_info->sp_start_y+1;
	edgecondition_areas[2]->orientation = edgecondition_areas[2]->VERTICAL;
	edgecondition_areas[2]->size = main_info->l_sp-1;
	edgecondition_areas[2]->start_consumption = -100.0f;

	// ������ �����
	edgecondition_areas[3] = new EDGECONDITION_AREA;
	edgecondition_areas[3]->name = "down_canal_in";
	edgecondition_areas[3]->coord.x = 1+edgecond_slash_from_edge;
	edgecondition_areas[3]->coord.y = main_info->sp_start_y+1;
	edgecondition_areas[3]->orientation = edgecondition_areas[3]->VERTICAL;
	edgecondition_areas[3]->size = main_info->l_sp-1;
	edgecondition_areas[3]->start_consumption = -100.0f;
	return true;
}

bool EdgeCondition::CountConsumptionInTheArea( EDGECONDITION_AREA* area, int num ) {

  double u_x; double u_y;
	int x, y;
	double n;
	area->consumption = 0;
	for( int l=0; l<area->size; l++ ) {
		x = area->coord.x;
		y = area->coord.y;
		switch ( area->orientation  ) {
		case EDGE_AREA::HORISONTAL:
			x += l;
			break;
		case EDGE_AREA::VERTICAL:
			y += l;
			break;
		}
		if( !Cells[x][y] || Cells[x][y]->type == 9 ) {
			printf("Error of consuption area\n");
			printf("numb=%i l=%i\n", num, l);
			goto next;
			return false;
		}
		else {
			u_x = 0.0f;
			u_y = 0.0f;
			n = 0;
			for(int i=0; i<MainInfo_->speed_quantity; i++) {
				for(int j=0; j<MainInfo_->speed_quantity; j++) {
					for(int k=0; k<MainInfo_->speed_quantity; k++) {
						if( Cells[x][y]->GetP(i,j,k) > MainInfo_->p_cut &&
							MainInfo_->FLAG_use_cut_sphere ) {
							// do nothing
						}
						else {
							//Cells[x][y]->real_SPEED_CUBE[i][j][k] = 0;
							n += Cells[x][y]->real_SPEED_CUBE[i][j][k];
							u_x += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
								Cells[x][y]->GetP(i)/Cells[x][y]->GetMolMass();
							u_y += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
								Cells[x][y]->GetP(j)/Cells[x][y]->GetMolMass();
						}
					}
				}
			}
			switch ( area->orientation  ) {
			case EDGE_AREA::HORISONTAL:
				//area->consumption += u_y / n;
				area->consumption += u_y;
				break;
			case EDGE_AREA::VERTICAL:
				//area->consumption += u_x / n;
				area->consumption += u_x;
				break;
			}			
		}
next:{}
	}
	if( area->start_consumption == -100.0f )
		area->start_consumption = area->consumption;
	return true;
}

bool EdgeCondition::CountAllConsuptions( void ) {
	if( !MainInfo_->FLAG_use_edge_conditions )
		return false;
	for( int i=0; i<edgecond_areas_quantity; i++ )
		CountConsumptionInTheArea( edgecondition_areas[i], i );
	return true;
}

bool EdgeCondition::FillEdgeAreas( void ) {
	if( !MainInfo_->FLAG_use_edge_conditions )
		return false;
	MAIN_INFO* main_info = MainInfo_->main_info;
	// �� ����� ����� ������
	// �� ������� � ������
	edge_areas = new EDGE_AREA*[4];
	edge_areas_quantity = 4;
	// ������ � ������ ������� (����� �������� ������)
	edge_areas[0] = new EDGE_AREA;
	edge_areas[0]->name = "up_canal_out";
	edge_areas[0]->coord.x = main_info->size_x-2;
	edge_areas[0]->coord.y = main_info->sp_start_y+main_info->l_sp+main_info->depth_of_the_gap;
	edge_areas[0]->orientation = edge_areas[0]->VERTICAL;
	edge_areas[0]->size = main_info->l_sp-1;

	// ����� �������
	edge_areas[1] = new EDGE_AREA;
	edge_areas[1]->name = "up_canal_in";
	edge_areas[1]->coord.x = 1;
	edge_areas[1]->coord.y = main_info->sp_start_y+main_info->l_sp+main_info->depth_of_the_gap;
	edge_areas[1]->orientation = edge_areas[1]->VERTICAL;
	edge_areas[1]->size = main_info->l_sp-1;

	// ������ ������
	edge_areas[2] = new EDGE_AREA;
	edge_areas[2]->name = "down_canal_out";
	edge_areas[2]->coord.x = main_info->size_x-2;
	edge_areas[2]->coord.y = main_info->sp_start_y+1;
	edge_areas[2]->orientation = edge_areas[2]->VERTICAL;
	edge_areas[2]->size = main_info->l_sp-1;

	// ������ �����
	edge_areas[3] = new EDGE_AREA;
	edge_areas[3]->name = "down_canal_in";
	edge_areas[3]->coord.x = 1;
	edge_areas[3]->coord.y = main_info->sp_start_y+1;
	edge_areas[3]->orientation = edge_areas[3]->VERTICAL;
	edge_areas[3]->size = main_info->l_sp-1;
	return true;
}

bool EdgeCondition::SetEdgeArea( EDGE_AREA* area, int num ) {
  double u_x = 0;
	double u_y = 0;
	double consumption = 0;
	int x, y;
	double C = 0.0f;
	double k;	
	//if( num == 1 || num == 3 )
	//	k = 6.0f / CUBE( area->size ) * edgecondition_areas[num]->start_consumption;
	//else
		k = 6.0f / CUBE( area->size ) * edgecondition_areas[num]->consumption;
	double p_slash;
	// �������� ���������� �������������
	for( int l=0; l<area->size; l++ ) {
		x = area->coord.x;
		y = area->coord.y;
		switch ( area->orientation  ) {
		case EDGE_AREA::HORISONTAL:
			x += l;
			break;
		case EDGE_AREA::VERTICAL:
			y += l;
			break;
		}
		if( !Cells[x][y] || Cells[x][y]->type == 9 ) {
			printf("Error of consuption area\n");
			return false;
		}
		else {
				C = 0.0f;
				// count C
				p_slash = -k*l*(l-area->size-1)*Cells[x][y]->GetMolMass();

				// ��� ����� (�����)
				p_slash /= Cells[x][y]->n;
				
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
				u_x = 0;
				u_y = 0;
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
									QUAD( Cells[x][y]->GetP(k) ) )  /
									(2.0f*Cells[x][y]->GetMolMass()*Cells[x][y]->T_start));
								u_x += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
									Cells[x][y]->GetP(i)/Cells[x][y]->GetMolMass();
								u_y += Cells[x][y]->real_SPEED_CUBE[i][j][k]*
									Cells[x][y]->GetP(j)/Cells[x][y]->GetMolMass();
							}
						}
					}
				}
			switch ( area->orientation  ) {
			case EDGE_AREA::HORISONTAL:
				consumption += u_y;
				break;
			case EDGE_AREA::VERTICAL:
				consumption += u_x;
				break;
			}			
		}
	}
	
	//// ��������	
	//printf("\rcons_check = %f cons = %f delta = %f",
	//	edgecondition_areas[num]->consumption, consumption,
	//	edgecondition_areas[num]->consumption - consumption );
	return true;
}

bool EdgeCondition::SetAllEdgeAreas( void ) {
	if( !MainInfo_->FLAG_use_edge_conditions )
		return false;
	for( int i=0; i<edge_areas_quantity; i++ )
		SetEdgeArea( edge_areas[i], i );
	return true;
}

bool EdgeCondition::SetEdgeConditions( void ) {
	if( !MainInfo_->FLAG_use_edge_conditions )
		return false;
	CountAllConsuptions();
	SetAllEdgeAreas();
	return true;
}
