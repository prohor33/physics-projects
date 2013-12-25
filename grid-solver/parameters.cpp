#include "parameters.h"

Parameters::Parameters() {}

void Parameters::Initialize() {

  // TODO: Should be here impulses, not velocities?

  // fill in velocities
  for (int i=0; i<speed_quantity_; i++) {
    velocity_.push_back(-p_cut_ +
        i * p_cut_ * 2.0f / (speed_quantity_-1));
  }

  // fill in velocities maps
  int index = 0;
  vector<int> coord(3);
  for (int i=0; i<speed_quantity_; i++) {
    for (int j=0; j<speed_quantity_; j++) {
      for (int k=0; k<speed_quantity_; k++) {

        // check for the out of cut sphere case
        if (sep::sqr(velocity_[i]) + sep::sqr(velocity_[j]) +
            sep::sqr(velocity_[k]) < sep::sqr(p_cut_)) {
          // inside

          coord[0] = i;
          coord[1] = j;
          coord[2] = k;
          s_coord_map_1d_to_3d_[index] = coord;
          s_coord_map_3d_to_1d_[coord] = index;
          index++;
        }
      }
    }
  }
}
