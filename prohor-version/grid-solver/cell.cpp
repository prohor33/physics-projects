#include "cell.h"
#include "parameters.h"

double Cell::MolMass() const {
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

double Cell::H() const {
  // TODO: maybe different cell to cell
  // and it should depend on direction (X, Y, Z)
  return 0.02;
}

double Cell::P(sep::Axis axis,
    vector<int> coord) const {
  // TODO: why is speed here?
  return PARAMETERS->velocity_[coord[axis]];
}

double Cell::Limiter(sep::Axis axis,
    vector<int> coord) {

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

vector<int> Cell::GetCoord(int index) {
  // TODO: return real 3d coordinates

  vector<int> coord(3);
  coord[sep::X] = 13;
  coord[sep::Y] = 21;
  coord[sep::Z] = 32;

  return coord;
}

int Cell::GetIndex(vector<int> coord) {
  // TODO: to implement
  return 13;
}


void Cell::ComputeHalfSpeed(double dt, sep::Axis axis) {

  if (type_ == FAKE || type_ == OBTAINED)
    return;

	if (!neighbor_[axis].next->neighbor_[axis].next ||
	    !neighbor_[axis].prev->neighbor_[axis].prev) {
	  // this is border cell, should invoke another function
	  // TODO: invoke another function
	  return;
	}

	double gamma;
	double p;

	vector<int> coord(3);
	vector<double>::iterator cii;

	for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

	  coord = GetCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    if (p > 0.0f) {
      // for speed > 0

      (*cii) = speed(coord) +
        ( 1.0f - fabs( gamma ) ) / 2.0f * Limiter(axis, coord);
    }
    else {
      // for speed < 0

      (*cii) = neighbor_[axis].next->speed(coord) -
        ( 1.0f - fabs( gamma ) ) / 2.0f *
        neighbor_[axis].next->Limiter(axis, coord);

	  }
	}
}


void Cell::ComputeSpeed(double dt, sep::Axis axis) {

  if (type_ == FAKE || type_ == OBTAINED)
    return;

  if (!neighbor_[axis].next->neighbor_[axis].next ||
      !neighbor_[axis].prev->neighbor_[axis].prev) {
    // this is border cell, should invoke another function
    // TODO: invoke another function
    return;
  }

  double gamma;
  double p;

  vector<int> coord(3);
  vector<double>::iterator cii;

  for (cii=speed_.begin(); cii!=speed_.end(); ++cii) {

    coord = GetCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    (*cii) = (*cii) -
        gamma * (speed_half(coord) - neighbor_[axis].prev->speed_half(coord));

  }

}

void Cell::ComputeHalfSpeedPrevIsBorder(double dt, sep::Axis axis) {

  double numenator1 = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;

  double gamma;
  double p;

  vector<int> coord(3);
  vector<double>::iterator cii;

  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    if (p > 0.0f) {
      // for speed > 0

      denominator += fabs(p / MolMass()) *
        exp((-1.0f)*p*p/(2.0f*MolMass()*wall_T_));
    }
    else {
      // for speed < 0

      neighbor_[axis].prev->speed(coord) =
        sep::max((double)0.0, 2.0f*speed(coord) - neighbor_[axis].next->speed(coord));

      neighbor_[axis].prev->speed_half(coord) = speed(coord) -
        (1.0 - fabs(gamma)) / 2.0f * Limiter(axis, coord);

      numenator1 += fabs(p / MolMass()) *
          neighbor_[axis].next->speed_half(coord);

      numenator2 += fabs(p / MolMass()) *
          0.5f * (neighbor_[axis].prev->speed(coord) + speed(coord));
    }

  }


  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    if (p > 0.0f) {
      // for speed > 0

      wall_T_ =
         numenator1 / denominator * exp((-1.0f) * p * p / (2.0f * MolMass() * wall_T_));

      MainInfo_->g_speed[x][y][z] =
        numenator2 / denominator * exp((-1.0f) * p * p / (2.0f * MolMass() * wall_T_));

      neighbor_[axis].prev->speed(coord) =
        sep::max((double)0.0, 2.0f*MainInfo_->g_speed[x][y][z] -
          speed(coord));

      speed_half(coord) = speed(coord) +
        (1.0f - fabs(gamma)) / 2.0f * neighbor_[axis].next->Limiter(axis, coord);
    }

  }

}
