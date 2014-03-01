#include "out_result.h"
#include "solver.h"
#include "grid.h"
#include "cell.h"
#include "parameters.h"
#include "flow_keeper.h"
#include <stdlib.h>


void OutResult::OutParameters(sep::GasNumb gas_numb) {

  OutParameter(sep::T, gas_numb);
  OutParameter(sep::n, gas_numb);
  OutParameter(sep::Flow, gas_numb);
}


void OutResult::OutParameter(sep::Parameter par, sep::GasNumb gas_numb) {
  if (PARAMETERS->GetUseParallelComputing()) {
    SetOutputType(sep::OUT_BINARY_FILE);
    OutParameterMPI(par, gas_numb);
  }
  else {
    SetOutputType(sep::OUT_TXT_FILE);
    OutParameterSingletone(par, gas_numb);
  }
}


void OutResult::OutParameterMPI(sep::Parameter par, sep::GasNumb gas_numb) {

  string par_str;
  string filename_str;

  switch (par) {
  case sep::T:
    par_str = string("T");
    break;
  case sep::n:
    par_str = string("n");
    break;
  case sep::Flow:
    par_str = string("Flow");
    break;
  }

  filename_str = string("../output/result_" + par_str + "_gas_" +
      sep::int_to_string(gas_numb) + ".dat");

  int rank, size;
  int parameters_q = parameters_[gas_numb].size();

  rank = PARAMETERS->GetProcessID();
  size = PARAMETERS->GetProcessesQ();

  MPI_Status status_par;
  int start_p = 0;

  // finding out start position for this process
  if (rank == 0) {

    remove(filename_str.c_str());

    MPI_Send(&parameters_q, 1, MPI_INT, rank+1, 123, MPI_COMM_WORLD);

  } else if (rank == size-1) {

    MPI_Recv(&start_p, 1, MPI_INT, rank-1, 123, MPI_COMM_WORLD,
        &status_par);

  } else {

    MPI_Recv(&start_p, 1, MPI_INT, rank-1, 123, MPI_COMM_WORLD,
        &status_par);
    int new_start_p = start_p + parameters_q;
    MPI_Send(&new_start_p, 1, MPI_INT, rank+1, 123, MPI_COMM_WORLD);

  }

  int par_q;
  switch (par) {
    case sep::T:
    case sep::n:
      par_q = 1;
      break;
    case sep::Flow:
      par_q = 3;
      break;
  }

  int par_size = sizeof(int)*3 + sizeof(double)*par_q;

  MPI_Offset offset = par_size * start_p;
  MPI_File file;
  MPI_Status status;

  // opening one shared file
  MPI_File_open(MPI_COMM_WORLD, (char*)filename_str.c_str(),
      MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &file);

  MPI_File_seek(file, offset, MPI_SEEK_SET);

  switch (output_type_) {

  case sep::OUT_BINARY_FILE: {

    vector<CellParameters>::iterator cii;

    for (cii=parameters_[gas_numb].begin();
        cii!=parameters_[gas_numb].end(); ++cii) {

      CellParameters& cell_par = *cii;

      double rank_d = rank;
      MPI_File_write(file, &cell_par.coord[0], 3, MPI_INT, &status);

      switch (par) {
        case sep::T:
          MPI_File_write(file, &cell_par.T, 1, MPI_DOUBLE, &status);
          break;
        case sep::n:
          MPI_File_write(file, &cell_par.n, 1, MPI_DOUBLE, &status);
          break;
        case sep::Flow:
          MPI_File_write(file, &cell_par.u[0], 3, MPI_DOUBLE, &status);
          break;
      }

    }
    break;
  }
  default:
    cout << "error: there is no such output type" << endl;
    break;
  }

  MPI_File_close(&file);
}


void OutResult::OutParameterSingletone(sep::Parameter par,
    sep::GasNumb gas_numb) {

  vector<Cell*> cells_z;

  ofstream out_par_file;

  string par_str;
  switch (par) {
    case sep::T:
      par_str = string("T");
      break;
    case sep::n:
      par_str= string("n");
      break;
    case sep::Flow:
     cout << "error: Flow doen't supported for txt file (singletone\
   case)" << endl;
     return;
 }

  out_par_file.open((string("../output/result_") + par_str + string("_gas") +
      sep::int_to_string(gas_numb) +
        ".txt").c_str());

  switch (output_type_) {

    case sep::OUT_TXT_FILE: {

      vector<CellParameters>::iterator cii;

      for (cii=parameters_[gas_numb].begin();
          cii!=parameters_[gas_numb].end(); ++cii) {

        double par_v;
        switch (par) {
          case sep::T:
            par_v = (*cii).T;
            break;
          case sep::n:
            par_v = (*cii).n;
            break;
          case sep::Flow:
            cout << "error: Flow doen't supported for txt file (singletone\
          case)" << endl;
            return;
        }

        out_par_file << (*cii).coord[sep::X] << " " <<
            (*cii).coord[sep::Y] << " " <<
            (*cii).coord[sep::Z] << " " <<
            par_v << endl;
      }
      break;
    }
    default:
      cout << "error: there is no such output type" << endl;
      break;
  }

  out_par_file.close();
}

// prepare parameters to be printed out
void OutResult::ProcessParameters(sep::GasNumb gas_numb) {

  int z_layer = SOLVER->grid()->size()[sep::Z]/2;
  z_layer = PARAMETERS->GetUseZAxis() ? z_layer : 0;

  // clean up previous results
  parameters_[gas_numb].clear();

  vector<double>::iterator cii;

  vector<int> coord(3);
  Cell* cell;
  const vector<int>& size = SOLVER->grid()->size();
  const vector<int>& start = SOLVER->grid()->start();
  vector<int> actual_cell_c(3);
  vector<double> u(3);

  for(int i=0; i<size[sep::X]; i++) {
    for(int j=0; j<size[sep::Y]; j++) {
      for(int k=0; k<size[sep::Z]; k++) {

        // will compute only one layer for a while
        if (k != z_layer)
          continue;

        cell = SOLVER->grid()->cells()[gas_numb][i][j][k];

        actual_cell_c = cell->space_coord();
        actual_cell_c[sep::X] += start[sep::X];
        actual_cell_c[sep::Y] += start[sep::Y];
        actual_cell_c[sep::Z] += start[sep::Z];

        u[sep::X] = 100.0;
        u[sep::Y] = 100.0;
        u[sep::Z] = 100.0;

        if (cell->obtained())
          continue;

        if (cell->type() != sep::NORMAL) {

          parameters_[gas_numb].push_back(
            CellParameters(actual_cell_c, 0.0, 0.0, u));

          continue;
        }

        // process T
        double n = 0.0;
        double T = 0.0;
        u[sep::X] = 0.0;
        u[sep::Y] = 0.0;
        u[sep::Z] = 0.0;

        // process constants
        for (cii=cell->speed_.begin();
          cii!=cell->speed_.end(); ++cii) {

          coord =
            cell->GetSpeedCoord((int)(cii-cell->speed_.begin()));

          n += cell->speed(coord);

          u[sep::X] += cell->speed(coord) *
              cell->P(sep::X, coord) / cell->MolMass();

          u[sep::Y] += cell->speed(coord) *
              cell->P(sep::Y, coord) / cell->MolMass();

          u[sep::Z] += cell->speed(coord) *
              cell->P(sep::Z, coord) / cell->MolMass();
        }

        // normalize
        u[sep::X] /= n; u[sep::Y] /= n; u[sep::Z] /= n;

        for (cii=cell->speed_.begin();
          cii!=cell->speed_.end(); ++cii) {

          coord =
            cell->GetSpeedCoord((int)(cii-cell->speed_.begin()));

          T += cell->MolMass() *
              (sep::sqr(cell->P(sep::X, coord) /
                        cell->MolMass() - u[sep::X])+
              sep::sqr(cell->P(sep::Y, coord) /
                cell->MolMass() - u[sep::Y])+
              sep::sqr(cell->P(sep::Z, coord) /
                cell->MolMass() - u[sep::Z])) *
              cell->speed(coord);
        }

        T /= n;
        // capacity
        T /= 3;

        parameters_[gas_numb].push_back(
            CellParameters(actual_cell_c, T, n, u));
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

        if (cell->type() != sep::NORMAL || cell->obtained())
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

void OutResult::OutPermanentParameters() {

  int rank = PARAMETERS->GetProcessID();

  if (PARAMETERS->GetUseFlowKeeper()) {
    if (FLOW_KEEPER->areas().size() < 2)
      return;

    static ofstream out_f[2];

    for (int i=0; i<2; i++) {
      FlowAreaData* area = FLOW_KEEPER->areas()[i];
      // TODO: rewrite this "code"
      if (!out_f[i].is_open()) {
        out_f[i].open(string("../output/cons_in_area_") +
            sep::int_to_string(rank*2+i) +
            string(".txt").c_str());
      }
      out_f[i] << area->consumption << endl;
    }
  }
}
