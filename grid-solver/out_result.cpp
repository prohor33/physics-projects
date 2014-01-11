#include "out_result.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"
#include "parameters.h"


void OutResult::OutParameters(sep::GasNumb gas_numb) {

  int z_layer = 5;

  z_layer = PARAMETERS->GetUseZAxis() ? z_layer : 0;

  vector<Cell*> cells_z;

  // out T
  ofstream out_T_file;
  out_T_file.open(string("test_T_gas" + sep::int_to_string(gas_numb) +
        ".result").c_str());

  switch (output_type_) {

  case OUT_FOR_PYTHON:

    vector<CellParameters>::iterator cii;

    for (cii=parameters_[gas_numb].begin();
        cii!=parameters_[gas_numb].end(); ++cii) {

      if ((*cii).coord[sep::Z] != z_layer)
        continue;

      out_T_file << (*cii).coord[sep::X] << " " <<
          (*cii).coord[sep::Y] << " " <<
          (*cii).coord[sep::Z] << " " <<
          (*cii).T << endl;
    }
    break;
  }

  out_T_file.close();


  // out n
  ofstream out_n_file;
  out_n_file.open(string("test_n_gas" + sep::int_to_string(gas_numb) +
        ".result").c_str());

  switch (output_type_) {

  case OUT_FOR_PYTHON:

    vector<CellParameters>::iterator cii;

    for (cii=parameters_[gas_numb].begin();
        cii!=parameters_[gas_numb].end(); ++cii) {

      if ((*cii).coord[sep::Z] != z_layer)
        continue;

      out_n_file << (*cii).coord[sep::X] << " " <<
          (*cii).coord[sep::Y] << " " <<
          (*cii).coord[sep::Z] << " " <<
          (*cii).n << endl;
    }
    break;
  }

  out_n_file.close();
}

// prepare parameters to be printed out
void OutResult::ProcessParameters(sep::GasNumb gas_numb) {

  vector<double>::iterator cii;

  vector<int> coord(3);
  Cell* cell;
  const vector<int>& size = SOLVER->grid()->size();

  for(int i=0; i<size[sep::X]; i++) {
    for(int j=0; j<size[sep::Y]; j++) {
      for(int k=0; k<size[sep::Z]; k++) {

        cell = SOLVER->grid()->cells()[gas_numb][i][j][k];

        if (cell->type() != Cell::NORMAL || cell->obtained()) {

          // TODO: make it global coordinate
          // actually we shoild develop another
          // way to output results in parallel case
          parameters_[gas_numb].push_back(
            CellParameters(cell->space_coord(), 0.0, 0.0));

          continue;
        }

        // process T
        double n = 0.0;
        double u_x = 0.0;
        double u_y = 0.0;
        double u_z = 0.0;
        double T = 0.0;

        // process constants
        for (cii=cell->speed_.begin();
          cii!=cell->speed_.end(); ++cii) {

          coord =
            cell->GetSpeedCoord((int)(cii-cell->speed_.begin()));

          n += cell->speed(coord);

          u_x += cell->speed(coord) *
              cell->P(sep::X, coord) / cell->MolMass();

          u_y += cell->speed(coord) *
              cell->P(sep::Y, coord) / cell->MolMass();

          u_z += cell->speed(coord) *
              cell->P(sep::Z, coord) / cell->MolMass();
        }

        // normalize
        u_x /= n; u_y /= n; u_z /= n;

        for (cii=cell->speed_.begin();
          cii!=cell->speed_.end(); ++cii) {

          coord =
            cell->GetSpeedCoord((int)(cii-cell->speed_.begin()));

          T += cell->MolMass() *
              (sep::sqr(cell->P(sep::X, coord) /
                        cell->MolMass() - u_x)+
              sep::sqr(cell->P(sep::Y, coord) /
                cell->MolMass() - u_y)+
              sep::sqr(cell->P(sep::Z, coord) /
                cell->MolMass() - u_z)) *
              cell->speed(coord);
        }

        T /= n;
        // capacity
        T /= 3;

        parameters_[gas_numb].push_back(
            CellParameters(cell->space_coord(), T, n));
      }
    }
  }
}

void OutResult::CheckMassConservation(sep::GasNumb gas_numb) {

  if (!PARAMETERS->GetUseCheckingMassConservation())
    return;

  vector<double>::iterator cii;

  vector<int> coord(3);

  double whole_mass = 0;

  Cell* cell;
  const vector<int>& size = SOLVER->grid()->size();

  for(int i=0; i<size[sep::X]; i++) {
    for(int j=0; j<size[sep::Y]; j++) {
      for(int k=0; k<size[sep::Z]; k++) {

        cell = SOLVER->grid()->cells()[gas_numb][i][j][k];

        if (cell->type() != Cell::NORMAL || cell->obtained())
          continue;

        double n = 0.0;

        // process constants
        for (cii=cell->speed_.begin();
          cii!=cell->speed_.end(); ++cii) {

          coord = cell->GetSpeedCoord(
              (int)(cii-cell->speed_.begin()));

          n += cell->speed(coord);
        }

        whole_mass += n;
      }
    }
  }

  if (whole_mass_ > 0) {
    if (whole_mass_ != whole_mass) {
      cout << "Error: checking mass conservation law failed." << endl;
      cout << "Gas: " << gas_numb << endl;
      cout << "difference = " << whole_mass - whole_mass_ << endl;
    }
  }
  else
    whole_mass_ = whole_mass;
}
