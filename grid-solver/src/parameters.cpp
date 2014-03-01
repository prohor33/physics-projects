#include "parameters.h"

Parameters::Parameters() :
  mol_mass_gas1_(-1.0),
  mol_mass_gas2_(-1.0),
  time_step_(0),
  limiter_(sep::Limiter(-1)),
  process_id_(-1),
  processes_q_(0),
  second_gas_is_active_(false),
  use_checking_mass_conservation_(false),
  use_z_axis_(false),
  use_parallel_computing_(false),
  use_grid_from_input_file_(false),
  use_collision_integral_(false),
  use_flow_keeper_(false),
  speed_quantity_(false),
  p_cut_(0),
  cout_backup_buf_(NULL),
  log_type_(sep::LogType(-1)),
  T1_(0.0),
  T2_(0.0),
  start_flow_(0.0)
{};


void Parameters::Initialize() {

  // TODO: Should be here impulses, not velocities?

  // fill in velocities
  for (int i=0; i<speed_quantity_; i++) {
    velocity_.push_back(-p_cut_ +
        i * p_cut_ * 2.0f / (speed_quantity_-1));
  }

  // fill in velocities maps
  int index = 0;
  s_coord_map_1d_to_3d_.clear();
  s_coord_map_3d_to_1d_.clear();
  vector<int> coord(4);
  for (int i=0; i<speed_quantity_; i++) {
    for (int j=0; j<speed_quantity_; j++) {
      for (int k=0; k<speed_quantity_; k++) {

        // check for the out of cut sphere case
        if (sep::sqr(velocity_[i]) + sep::sqr(velocity_[j]) +
            sep::sqr(velocity_[k]) < sep::sqr(p_cut_)) {
          // inside

          coord[sep::X] = i;
          coord[sep::Y] = j;
          coord[sep::Z] = k;
          coord[sep::INDEX] = index;  // just in case
          s_coord_map_1d_to_3d_.push_back(coord);
          s_coord_map_3d_to_1d_[coord] = index;
          index++;
        }
      }
    }
  }
}

void Parameters::SetLogType(sep::LogType ltype) {
  switch (ltype) {
  case sep::CONSOLE:
    if (cout_backup_buf_) {
      cout.rdbuf(cout_backup_buf_); // restore buffer
      log_file_str_.close();
    }
    break;
  case sep::LOG_FILE:
    stringstream log_filename;
    time_t t = time(0);
    struct tm * now = localtime( & t );

    log_filename << "../logs/"
    <<  now->tm_mday << '-'
    << (now->tm_mon + 1) << '-'
    << (now->tm_year + 1900) << '_'
    << now->tm_hour << ':'
    << now->tm_min
    << ".log";

    log_file_str_.open (log_filename.str());
    cout_backup_buf_ = cout.rdbuf();
    log_file_buf_ = log_file_str_.rdbuf();
    cout.rdbuf(log_file_buf_);

    break;
  }
  log_type_ = ltype;
}

void Parameters::OutTime() {
    time_t t = time(0);
    struct tm * now = localtime( & t );

    cout << now->tm_hour << ':'
    << now->tm_min << ':'
    << now->tm_sec
    << " : ";
}
