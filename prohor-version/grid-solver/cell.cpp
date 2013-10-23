#include "cell.h"
#include "parameters.h"

double Cell::MolMass() {
  switch(gas_numb_) {
  case 1:
    return PARAMETERS->mol_mass_gas1_;
    break;
  case 2:
    return PARAMETERS->mol_mass_gas1_;
    break;
  }
  return 0;
}

double Cell::H() {
  return 0.02;
}

double Cell::P(sep::Axis axis,
    const int* coord) {
  return PARAMETERS->velocity_[coord[axis]];
}

double Cell::Limiter(sep::Axis axis,
    const int* coord) {
  if( !neighbor_[axis].next || !neighbor_[axis].prev ) {
    cout << "error limiter" << endl;
    return -1.0f;
  }
  double val = speed(coord);
  double next = neighbor_[axis].next->speed(coord);
  double prev = neighbor_[axis].prev->speed(coord);

  switch (PARAMETERS->limiter_) {
  case sep::SUPERBEE:
    return sep::max(sep::min(2.0f*sep::module(next - val),
        sep::module(val - prev)),
        sep::min( sep::module(next - val),
        2.0f*sep::module(val - prev)));
    break;
  case sep::MC:
    return sep::min(
        sep::min(sep::module(next - prev) / 2.0f,
        2.0f * sep::module(next - val)),
        2.0f * sep::module(val - prev)) *
        sep::sign(next - val);
    break;
  case sep::STRANGE:
    return sep::max((double)0.0f, sep::min(2.0f*sep::module(val - prev),
        sep::min(sep::module(next - val),
        sep::min(sep::module(val - prev),
        2.0f*sep::module(next - val))))) *
        sep::sign(next - val);
    break;
  default:
    cout << "error limiter" << endl;
    return 0.0f;
    break;
  }
  return 0;
}

void Cell::ComputeHalfSpeed(double dt, sep::Axis axis) {
	if (!neighbor_[axis].next->neighbor_[axis].next ||
	    !neighbor_[axis].prev->neighbor_[axis].prev) {
	  // this is border cell, should invoke another function
	  return;
	}
	double gamma;
	double p;
	// TODO: should we replace this block with FOR_EACH?
	vector<vector<vector<double> > >::iterator cii_x;
	vector<vector<double> >::iterator cii_xy;
	vector<double>::iterator cii_xyz;
	int coord[3];
	for (cii_x=speed_half_.begin(); cii_x!=speed_half_.end(); ++cii_x) {
	  for (cii_xy=(*cii_x).begin(); cii_xy!=(*cii_x).end(); ++cii_xy) {
	    for (cii_xyz=(*cii_xy).begin(); cii_xyz!=(*cii_xy).end(); ++cii_xyz) {
        coord[sep::X] = cii_x - speed_half_.begin();
        coord[sep::Y] = cii_xy - (*cii_x).begin();
        coord[sep::Z] = cii_xyz - (*cii_xy).begin();
	      p = P(axis, coord);
	      gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();
        if (p > 0.0f) {
          // for speed > 0
          (*cii_xyz) = speed(coord) +
            ( 1.0f - fabs( gamma ) ) / 2.0f * Limiter(axis, coord);
        }
        else {
          // for speed < 0
          (*cii_xyz) = neighbor_[axis].next->speed(coord) -
            ( 1.0f - fabs( gamma ) ) / 2.0f *
            neighbor_[axis].next->Limiter(axis, coord);
        }
	    }
	  }
	}
}
