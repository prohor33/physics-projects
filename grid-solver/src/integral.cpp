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

  std::cout << "counting integral..." << std::endl;

  double m1 = PARAMETERS->GetMolMass(sep::FIRST);
  double m2 = PARAMETERS->GetMolMass(sep::SECOND);

  Cell *cell1, *cell2;
  const vector<int>& size = SOLVER->grid()->size();

  // 1<->1
  {
    Generate(PARAMETERS->time_step()*100, m1, m1, particle, particle);

    for(int i=0; i<size[sep::X]; i++) {
      for(int j=0; j<size[sep::Y]; j++) {
        for(int k=0; k<size[sep::Z]; k++) {

          cell1 = SOLVER->grid()->cells()[0][i][j][k];

          if (cell1->type() != sep::NORMAL)
            continue;

          FillInput(cell1, NULL);
          ci::iter(input_for_ci1, input_for_ci1);
          GetInput(cell1, NULL);
        }
      }
    }
  }

  if (!PARAMETERS->GetSecondGasIsActive())
    return;

  // 1<->2 & 2<->1
  {
    Generate(2.0 * PARAMETERS->time_step(), m1, m2, particle, particle);

    for(int i=0; i<size[sep::X]; i++) {
      for(int j=0; j<size[sep::Y]; j++) {
        for(int k=0; k<size[sep::Z]; k++) {

          cell1 = SOLVER->grid()->cells()[0][i][j][k];
          cell2 = SOLVER->grid()->cells()[1][i][j][k];

          if (cell1->type() != sep::NORMAL)
            continue;

          FillInput(cell1, cell2);
          ci::iter(input_for_ci1, input_for_ci2);
          FillInput(cell1, cell2);
        }
      }
    }
  }

  // 2<->2
  {
    Generate(PARAMETERS->time_step(), m2, m2, particle, particle);

    for(int i=0; i<size[sep::X]; i++) {
      for(int j=0; j<size[sep::Y]; j++) {
        for(int k=0; k<size[sep::Z]; k++) {

          cell2 = SOLVER->grid()->cells()[1][i][j][k];

          if (cell2->type() != sep::NORMAL)
            continue;

          FillInput(NULL, cell2);
          ci::iter(input_for_ci2, input_for_ci2);
          GetInput(NULL, cell2);
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
    if (cell2) {
      input_for_ci2[xyz2i[x][y][z]] =
        cell2->speed()[cii-cell1->speed().begin()];
    }
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

    if (cell2) {
      cell2->speed()[cii-cell1->speed().begin()] =
        input_for_ci2[xyz2i[x][y][z]];
    }
  }
}

void Integral::Generate( double time_step, double m1, double m2,  ci::Particle p1, ci::Particle p2 ) {

  ci::gen(time_step, 50000, radius, radius,  xyz2i, xyz2i, a, m1, m2,
			p1, p2);
  return;
}
