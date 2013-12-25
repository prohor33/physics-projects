#include "integral.h"
#include "parameters.h"
#include "solver.h"
#include "main.h"
#include "cell.h"

Integral::Integral() {

  ci::init(&potential, ci::NO_SYMM);

  particle.d = 1.;

  int speed_quantity = PARAMETERS->GetSpeedQuantity();
  double p_cut = PARAMETERS->GetPCut();

  radius = speed_quantity / 2.0f;
  a = p_cut / radius;

  xyz2i = new int**[speed_quantity];


  for( int i=0; i<speed_quantity; i++ ) {

    xyz2i[i] = new int*[speed_quantity];

      for( int j=0; j<speed_quantity; j++ ) {

      xyz2i[i][j] = new int[speed_quantity];

      for( int k=0; k<speed_quantity; k++ ) {

        xyz2i[i][j][k] = i + 2*radius*(j + 2*radius*k);
      }
    }
  }

  input_for_ci1 = new double[ ( 2*radius )*( 2*radius )*( 2*radius ) ];
  input_for_ci2 = new double[ ( 2*radius )*( 2*radius )*( 2*radius ) ];
  return;
};

void Integral::Iteration() {

  // Warning! Low quality code! Needed to be rewritten!

  std::cout << "counting integral..." << std::endl;

  double m1 = PARAMETERS->GetMolMass(sep::FIRST);
  double m2 = PARAMETERS->GetMolMass(sep::SECOND);

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  // 1<->1
  {
    Generate(PARAMETERS->time_step(), m1, m1, particle, particle);

    // TODO: need to develop some new way to surf throw cells!

    for (cii_x=SOLVER->grid()->cells(sep::FIRST).begin();
        cii_x!=SOLVER->grid()->cells(sep::FIRST).end(); ++cii_x) {

      for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
        for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {

          if ((*cii_xyz)->type() != Cell::NORMAL)
            continue;

          FillInput(*cii_xyz, NULL);
          ci::iter(input_for_ci1, input_for_ci1);
          GetInput(*cii_xyz, NULL);
        }
      }
    }
  }

  if (!PARAMETERS->GetSecondGasIsActive())
    return;

  // 1<->2 & 2<->1
  {
    Generate(2.0 * PARAMETERS->time_step(), m1, m2, particle, particle);

    for (cii_x=SOLVER->grid()->cells(sep::FIRST).begin();
        cii_x!=SOLVER->grid()->cells(sep::FIRST).end(); ++cii_x) {

      for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
        for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {

          if ((*cii_xyz)->type() != Cell::NORMAL)
            continue;

          int x = cii_xyz - (*cii_xy).begin();
          int y = cii_xy - (*cii_x).begin();
          int z = cii_x - SOLVER->grid()->cells(sep::FIRST).begin();
          Cell* cell2 = SOLVER->grid()->cells(sep::SECOND)[x][y][z];

          FillInput(*cii_xyz, cell2);
          ci::iter(input_for_ci1, input_for_ci2);
          FillInput(*cii_xyz, cell2);
        }
      }
    }
  }
  // 2<->2
  {
    Generate(PARAMETERS->time_step(), m2, m2, particle, particle);

    for (cii_x=SOLVER->grid()->cells(sep::SECOND).begin();
        cii_x!=SOLVER->grid()->cells(sep::SECOND).end(); ++cii_x) {

      for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
        for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {

          if ((*cii_xyz)->type() != Cell::NORMAL)
            continue;

          FillInput(NULL, *cii_xyz);
          ci::iter(input_for_ci2, input_for_ci2);
          GetInput(NULL, *cii_xyz);
        }
      }
    }
  }
  return;
}


void Integral::FillInput(Cell* cell_gas1, Cell* cell_gas2) {

  Cell* cell1;
  Cell* cell2;
  cell2 = NULL;

  if (!cell_gas1 && !cell_gas2)
    return;

  if (cell_gas1) {
    cell1 = cell_gas1;
    cell2 = cell_gas2;
  } else {
    cell1 = cell_gas2;
    cell2 = cell_gas1;
  }

  vector<double>::iterator cii;
  vector<int> coord;

  for (cii=cell1->speed().begin();
    cii!=cell1->speed().end(); ++cii) {

    coord =
      cell1->GetSpeedCoord((int)(cii-cell1->speed().begin()));

    int x = coord[sep::X];
    int y = coord[sep::Y];
    int z = coord[sep::Z];

    input_for_ci1[xyz2i[x][y][z]] = (*cii);
    if (cell2)
      input_for_ci2[xyz2i[x][y][z]] = cell2->speed()[cii-cell1->speed().begin()];
  }
}


void Integral::GetInput(Cell* cell_gas1, Cell* cell_gas2) {

  Cell* cell1;
  Cell* cell2;

  if (!cell_gas1 && !cell_gas2)
    return;

  if (cell_gas1) {
    cell1 = cell_gas1;
    cell2 = cell_gas2;
  } else {
    cell1 = cell_gas2;
    cell2 = cell_gas1;
  }

  vector<double>::iterator cii;
  vector<int> coord;

  for (cii=cell1->speed().begin();
    cii!=cell1->speed().end(); ++cii) {

    coord =
      cell1->GetSpeedCoord((int)(cii-cell1->speed().begin()));

    int x = coord[sep::X];
    int y = coord[sep::Y];
    int z = coord[sep::Z];

    (*cii) = input_for_ci1[xyz2i[x][y][z]];

    if (cell2)
      cell2->speed()[cii-cell1->speed().begin()] = input_for_ci2[xyz2i[x][y][z]];
  }
}

void Integral::Generate( double time_step, double m1, double m2,  ci::Particle p1, ci::Particle p2 ) {

  ci::gen(time_step, 50000, radius, radius,  xyz2i, xyz2i, a, m1, m2,
			p1, p2);
  return;
}
