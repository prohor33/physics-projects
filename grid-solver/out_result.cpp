#include "out_result.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"


void OutResult::OutParameters(sep::GasNumb gas_numb) {

  // out T
  ofstream out_T_file;
  out_T_file.open(string("test_T_gas" + sep::int_to_string(gas_numb) + ".result").c_str());

  switch (output_type_) {

  case OUT_FOR_PYTHON:

    vector<CellParameters>::iterator cii;

    for (cii=parameters_[gas_numb].begin();
        cii!=parameters_[gas_numb].end(); ++cii) {

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
  out_n_file.open(string("test_n_gas" + sep::int_to_string(gas_numb) + ".result").c_str());

  switch (output_type_) {

  case OUT_FOR_PYTHON:

    vector<CellParameters>::iterator cii;

    for (cii=parameters_[gas_numb].begin();
        cii!=parameters_[gas_numb].end(); ++cii) {

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

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  vector<double>::iterator cii;

  vector<int> coord(3);

  for (cii_x=SOLVER->grid_->cells(gas_numb).begin();
      cii_x!=SOLVER->grid_->cells(gas_numb).end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {

        if ((*cii_xyz)->type() != Cell::NORMAL)
          continue;

        // process T
        double n = 0.0;
        double u_x = 0.0;
        double u_y = 0.0;
        double u_z = 0.0;
        double T = 0.0;

        // process constants
        for (cii=(*cii_xyz)->speed_.begin();
          cii!=(*cii_xyz)->speed_.end(); ++cii) {

          coord = (*cii_xyz)->GetSpeedCoord((int)(cii-(*cii_xyz)->speed_.begin()));

          n += (*cii_xyz)->speed(coord);

          u_x += (*cii_xyz)->speed(coord) *
              (*cii_xyz)->P(sep::X, coord) / (*cii_xyz)->MolMass();

          u_y += (*cii_xyz)->speed(coord) *
              (*cii_xyz)->P(sep::Y, coord) / (*cii_xyz)->MolMass();

          u_z += (*cii_xyz)->speed(coord) *
              (*cii_xyz)->P(sep::Z, coord) / (*cii_xyz)->MolMass();
        }

        // normalize
        u_x /= n; u_y /= n; u_z /= n;

        for (cii=(*cii_xyz)->speed_.begin();
          cii!=(*cii_xyz)->speed_.end(); ++cii) {

          coord = (*cii_xyz)->GetSpeedCoord((int)(cii-(*cii_xyz)->speed_.begin()));

          T += (*cii_xyz)->MolMass() *
              (sep::sqr((*cii_xyz)->P(sep::X, coord) / (*cii_xyz)->MolMass() - u_x)+
              sep::sqr((*cii_xyz)->P(sep::Y, coord) / (*cii_xyz)->MolMass() - u_y)+
              sep::sqr((*cii_xyz)->P(sep::Z, coord) / (*cii_xyz)->MolMass() - u_z)) *
              (*cii_xyz)->speed(coord);
        }

        T /= n;
        // capacity
        T /= 3;

        parameters_[gas_numb].push_back(CellParameters((*cii_xyz)->space_coord(), T, n));
      }
    }
  }
}

void OutResult::CheckMassConservation(sep::GasNumb gas_numb) {

  vector<vector<vector<Cell*> > >::iterator cii_x;
  vector<vector<Cell*> >::iterator cii_xy;
  vector<Cell*>::iterator cii_xyz;

  vector<double>::iterator cii;

  vector<int> coord(3);

  double whole_mass = 0;

  for (cii_x=SOLVER->grid_->cells(gas_numb).begin();
      cii_x!=SOLVER->grid_->cells(gas_numb).end(); ++cii_x) {
    for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
      for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {

        if ((*cii_xyz)->type() != Cell::NORMAL)
          continue;

        double n = 0.0;

        // process constants
        for (cii=(*cii_xyz)->speed_.begin();
          cii!=(*cii_xyz)->speed_.end(); ++cii) {

          coord = (*cii_xyz)->GetSpeedCoord((int)(cii-(*cii_xyz)->speed_.begin()));

          n += (*cii_xyz)->speed(coord);
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
