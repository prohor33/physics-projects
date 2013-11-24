#include "parameters.h"

Parameters::Parameters() {

  double speed_cut = 4.8f;

  // fill in velocities
  int speed_quantity = 8; // for debug (should be 20)
  for (int i=0; i<speed_quantity; i++) {
    velocity_.push_back(-speed_cut + i * speed_cut * 2.0f / (speed_quantity-1));
  }

  // fill in velocities maps
  int index = 0;
  vector<int> coord(3);
  for (int i=0; i<speed_quantity; i++) {
    for (int j=0; j<speed_quantity; j++) {
      for (int k=0; k<speed_quantity; k++) {

        // check for the out of cut sphere case
        if (sep::sqr(velocity_[i]) + sep::sqr(velocity_[j]) +
            sep::sqr(velocity_[k]) < sep::sqr(speed_cut)) {
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
