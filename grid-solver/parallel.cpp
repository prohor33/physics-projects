#include "parallel.h"
#include "parameters.h"
#include "solver.h"
#include "cell.h"

void Parallel::InitializeMPI(int argc, char *argv[]) {

  MPI_Init (&argc, &argv);  /* starts MPI */

  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);  /* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);  /* get number of processes */

  PARAMETERS->SetUseParallelComputing(size != 1);

  if (PARAMETERS->GetUseParallelComputing()) {
    cout << "size = " << size << endl;
    cout << "rank = " << rank << endl;
  }

  PARAMETERS->SetProcessID(rank);
  PARAMETERS->SetProcessesQ(size);
}

void Parallel::FinalizeMPI() {
  MPI_Finalize();
}

void Parallel::ExchangeEdgeZone(sep::SpeedType sp_type) {

  int rank = PARAMETERS->GetProcessID();

//  cout << "r: " << rank << " start exchanging!" << endl;

  int next, prev;
  vector<int> start_s(3);
  vector<int> size(3);

  for (sep::Axis ax=sep::X; ax<=sep::Z; ax = (sep::Axis)((int)ax + 1)) {

    next = SOLVER->GetGridNeighbors()[ax].next;
    prev = SOLVER->GetGridNeighbors()[ax].prev;

    size = SOLVER->grid()->size();
    size[ax] = 2;

    start_s[sep::X] = 0;
    start_s[sep::Y] = 0;
    start_s[sep::Z] = 0;

    if (rank % 2 == 0) {
      // for even rank

      // SendRecieve with next first
      if (next != -1) {

//        cout << "r: " << rank << " SendRecieve with next first." << endl;
        start_s[ax] = SOLVER->grid()->size()[ax] - 4; // or should be -3??
        SendRecieveCellsBlock(start_s, size, 2, ax, sp_type, next);
      }

      // SendRecieve with prev after that
      if (prev != -1) {

//        cout << "r: " << rank << " SendRecieve with prev after that." << endl;
        start_s[ax] = 2;
        SendRecieveCellsBlock(start_s, size, -2, ax, sp_type, prev);
      }
    } else {
      // for odd rank

      // SendRecieve with prev first
      if (prev != -1) {

//        cout << "r: " << rank << " SendRecieve with prev first." << endl;
        start_s[ax] = 2;
        SendRecieveCellsBlock(start_s, size, -2, ax, sp_type, prev);
      }

      // SendRecieve with next after that
      if (next != -1) {

//        cout << "r: " << rank << " SendRecieve with next after that." << endl;
        start_s[ax] = SOLVER->grid()->size()[ax] - 4;
        SendRecieveCellsBlock(start_s, size, 2, ax, sp_type, next);
      }
    }
  }
}

void Parallel::SendRecieveCellsBlock(vector<int>& start_s, vector<int>& size,
                                     int shift, sep::Axis axis,
                                     sep::SpeedType sp_type, int dest) {

  Cell *cell_s, *cell_r;
  vector<int> cell_send_c(3);
  vector<int> cell_recieve_c(3);

  int rank = PARAMETERS->GetProcessID();

//  cout << "r: " << rank << " start_s[X] = " << start_s[axis] <<
//      " grid.size()[X] = " << SOLVER->grid()->size()[axis] <<
//      " axis = " << axis << endl;
//
//
//  cout << "r: " << PARAMETERS->GetProcessID() << " SendRecvCellBlock" << endl;
//  cout << "r: " << PARAMETERS->GetProcessID() << " start_s = " <<
//      start_s[sep::X] << endl;
//  cout << "r: " << PARAMETERS->GetProcessID() << " axis = " << axis << endl;

  // for each cell should do Sendrecv()
  for (int i=0; i<size[sep::X]; i++) {
    for (int j=0; j<size[sep::Y]; j++) {
      for (int k=0; k<size[sep::Z]; k++) {

        cell_send_c[sep::X] = i + start_s[sep::X];
        cell_send_c[sep::Y] = j + start_s[sep::Y];
        cell_send_c[sep::Z] = k + start_s[sep::Z];

        cell_recieve_c = cell_send_c;
        cell_recieve_c[axis] += shift;

        SendRecieveCell(cell_send_c, cell_recieve_c, sp_type, dest);
      }
    }
  }

//  cout << "rank: " << PARAMETERS->GetProcessID() <<
//      " succesfully send and recieve all data with dest = " << dest << endl;

}


void Parallel::SendRecieveCell(vector<int> cell_s_c,
                               vector<int> cell_r_c,
                               sep::SpeedType sp_type, int dest) {

  Cell *cell_s, *cell_r;
  int tag;
  int rank = PARAMETERS->GetProcessID();
  MPI_Status status;
  int recieved_n = 0;
  int res;

  tag = 123;

  // for each gas
  for (int gas_numb=0; gas_numb<(PARAMETERS->GetSecondGasIsActive() ? 2 : 1);
       gas_numb++) {

    cell_s = SOLVER->grid()->cells()[gas_numb][cell_s_c[sep::X]]
        [cell_s_c[sep::Y]][cell_s_c[sep::Z]];
    cell_r = SOLVER->grid()->cells()[gas_numb][cell_r_c[sep::X]]
        [cell_r_c[sep::Y]][cell_r_c[sep::Z]];

    vector<double>& speed_s = sp_type != sep::HalfSpeed ? cell_s->speed() :
        cell_s->speed_half();

    vector<double>& speed_r = sp_type != sep::HalfSpeed ? cell_r->speed() :
        cell_r->speed_half();

//    cout << "r: " << rank << " Want to sent: " <<
//        "coord_s: " << cell_s_c[sep::X] << " " <<
//        cell_s_c[sep::Y] << " " <<
//        cell_s_c[sep::Z] <<
//        " cell_s::type: " << cell_s->type() << endl;
//    cout << "r: " << rank << " Want to recieve: " <<
//        "coord_r: " << cell_r_c[sep::X] << " " <<
//        cell_r_c[sep::Y] << " " <<
//        cell_r_c[sep::Z] <<
//        " cell_r::type: " << cell_r->type() << endl;

    res = MPI_Sendrecv(&speed_s[0], speed_s.size(),
                       MPI_DOUBLE, dest, tag,
                       &speed_r[0], speed_r.size(),
                       MPI_DOUBLE, dest, tag,
                       MPI_COMM_WORLD, &status);

    if (res != MPI_SUCCESS) {
      cout << "error: MPI_Sendrecv finished unsuccessfully." <<
          " res = " << res << endl;
    }

    MPI_Get_count(&status, MPI_DOUBLE, &recieved_n);

    if (recieved_n != speed_r.size()) {
      cout << "r: " << PARAMETERS->GetProcessID() <<
          " error: recieve only " << recieved_n << " doubles. " <<
         "But should recieve " <<
         speed_r.size() << " doubles." << endl;

      cout << "Some additional information: " << endl <<
          "coord_r: " << cell_r_c[sep::X] << " " <<
          cell_r_c[sep::Y] << " " <<
          cell_r_c[sep::Z] << endl <<
          "cell_r::type: " << cell_r->type() << endl;

      exit(-1);
    }
  }
}
