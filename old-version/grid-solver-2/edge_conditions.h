#pragma once
#include"main.h"
#include"main_info.h"
#include"cell.h"

class EdgeCondition {
public:
	EdgeCondition( Cell*** Cells, int gas_index ){
		this->Cells = Cells;
		this->gas_index = gas_index;
		edgecond_slash_from_edge = 5;		
	};
	bool FillEdgeConditionAreas( void );
	bool FillEdgeAreas( void );
	bool CountAllConsuptions( void );
	struct EDGECONDITION_AREA {
		enum {
			HORISONTAL, VERTICAL
		} orientation;
		COORD_int coord;
		int size;
		string name;
		double consumption;
		double start_consumption;
	};
	struct EDGE_AREA {
		enum {
			HORISONTAL, VERTICAL
		} orientation;
		COORD_int coord;
		int size;
		string name;
	};
	bool CountConsumptionInTheArea( EDGECONDITION_AREA* area, int num );
	bool SetAllEdgeAreas( void );
	bool SetEdgeArea( EDGE_AREA* area, int num );
	bool SetEdgeConditions( void );
	EDGECONDITION_AREA** edgecondition_areas;
	EDGE_AREA** edge_areas;
	int edgecond_areas_quantity;
	int edge_areas_quantity;
	int edgecond_slash_from_edge;
	Cell*** Cells;
	int gas_index;
};

extern EdgeCondition* EdgeCondition1_;
extern EdgeCondition* EdgeCondition2_;