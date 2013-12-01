#include "cell.h"
#include "parameters.h"

#include <stdlib.h>

Cell::Cell(sep::GasNumb gas_numb, CellType type) :
    gas_numb_(gas_numb),
    type_(type)
  {

  // TODO: make it Gauss function
  vector<int> coord;
  double n = 1.0;
  double T_start = 1.0;
  double C = 0;

  // process C
  for (int i=0; i<PARAMETERS->s_coord_map_1d_to_3d_.size(); i++) {

    coord = PARAMETERS->s_coord_map_1d_to_3d_[i];

    C += exp((-1.0f)*
        (sep::sqr(P(sep::X, coord)) +
        sep::sqr(P(sep::Y, coord)) +
        sep::sqr(P(sep::Z, coord)) /
      (2.0f*MolMass()*T_start)));
  }

  /* n0 = 1.0 */
  C = 1.0f / C;

  for (int i=0; i<PARAMETERS->s_coord_map_1d_to_3d_.size(); i++) {

    coord = PARAMETERS->s_coord_map_1d_to_3d_[i];

    speed_.push_back(n * C * exp((-1.0f)*
        (sep::sqr(P(sep::X, coord)) +
        sep::sqr(P(sep::Y, coord)) +
        sep::sqr(P(sep::Z, coord))) /
        (2.0f*MolMass()*T_start)));

    //cout << "speed = " << speed_[i] << endl;
  }

  // initialize speed_half array
  speed_half_ = vector<double>(PARAMETERS->s_coord_map_1d_to_3d_.size());
};

double Cell::MolMass() const {
  switch(gas_numb_) {
  case 0:
    return PARAMETERS->mol_mass_gas1_;
    break;
  case 1:
    return PARAMETERS->mol_mass_gas2_;
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

  return PARAMETERS->velocity_[coord[axis]] * MolMass();
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

vector<int> Cell::GetSpeedCoord(int index) {

  if (index > PARAMETERS->s_coord_map_1d_to_3d_.size()-1) {

    cout << "Error in GetSpeedCoord: too much index = " <<
        index << " while map size = " <<
        PARAMETERS->s_coord_map_1d_to_3d_.size() << endl;
    return vector<int>(3);
  }
  return PARAMETERS->s_coord_map_1d_to_3d_[index];
}

int Cell::GetIndex(vector<int> coord) {

  return PARAMETERS->s_coord_map_3d_to_1d_[coord];
}


void Cell::ComputeHalfSpeed(sep::Axis axis, double dt) {

  if (type_ == FAKE || type_ == OBTAINED) {
    return;
  }

	if (!neighbor_[axis].next->neighbor_[axis].next) {
	  ComputeHalfSpeedNextIsBorder(axis, dt);
	  return;
	}

	if (!neighbor_[axis].prev->neighbor_[axis].prev) {
	  ComputeHalfSpeedPrevIsBorder(axis, dt);
	  return;
	}

	double gamma;
	double p;

	vector<int> coord(3);
	vector<double>::iterator cii;

	for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

	  coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    if (p > 0.0f) {
      // for speed > 0

      (*cii) = speed(coord) +
        (1.0f - fabs(gamma)) / 2.0f * Limiter(axis, coord);
    }
    else {
      // for speed < 0

      (*cii) = neighbor_[axis].next->speed(coord) -
        (1.0f - fabs(gamma)) / 2.0f *
        neighbor_[axis].next->Limiter(axis, coord);
	  }
	}
}


void Cell::ComputeSpeed(sep::Axis axis, double dt) {

  if (type_ == FAKE || type_ == OBTAINED) {
    return;
  }

  double gamma;
  double p;

  vector<int> coord(3);
  vector<double>::iterator cii;

  for (cii=speed_.begin(); cii!=speed_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_.begin()));

    p = P(axis, coord);

    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    speed(coord) += -
        gamma * (speed_half(coord) - neighbor_[axis].prev->speed_half(coord));
  }
}

void Cell::ComputeHalfSpeedPrevIsBorder(sep::Axis axis, double dt) {

  double numenator1 = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;

  double gamma;
  double p;

  vector<int> coord(3);
  vector<double>::iterator cii;

  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    // for speed < 0 with (18.5) compute prev->speed()

    if (p > 0.0f) {
      // for speed > 0

      denominator += fabs(p / MolMass()) *
        exp((-1.0f) * sep::sqr(p) / (2.0f * MolMass() * wall_t_));
    }
    else {
      // for speed < 0

      neighbor_[axis].prev->speed(coord) =
        sep::max((double)0.0, 2.0f * speed(coord) - neighbor_[axis].next->speed(coord));

      neighbor_[axis].prev->speed_half(coord) = speed(coord) -
        (1.0 - fabs(gamma)) / 2.0f * Limiter(axis, coord);

      numenator1 += fabs(p / MolMass()) *
          neighbor_[axis].prev->speed_half(coord);

      numenator2 += fabs(p / MolMass()) *
          0.5f * (neighbor_[axis].prev->speed(coord) + speed(coord));
    }

  }


  // for speed < 0 compute prev->speed_half() (f1/2)

  // prev->speed_half() (f1/2) for speed < 0 put into (20.5)
  // then find prev->speed_half() (f1/2) for speed > 0
  // it's diffuse reflection

  // put (20.6) into (20.5) instead of f1/2 for speed < 0
  // in that way we find g for speed > 0

  // then put g for speed > 0 into (18.3) ->
  // obtain prev->speed() for speed > 0
  // that allow to compute speed_half() (f3/2) for speed > 0
  // for speed < 0 we already could compute it earlier

  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();
    double g;

    if (p > 0.0f) {
      // for speed > 0

      neighbor_[axis].prev->speed_half(coord) =
         numenator1 / denominator * exp((-1.0f) * sep::sqr(p) / (2.0f * MolMass() * wall_t_));

      g = numenator2 / denominator * exp((-1.0f) * sep::sqr(p) / (2.0f * MolMass() * wall_t_));

      neighbor_[axis].prev->speed(coord) =
        sep::max((double)0.0, 2.0f * g - speed(coord));

      speed_half(coord) = speed(coord) +
        (1.0f - fabs(gamma)) / 2.0f * neighbor_[axis].next->Limiter(axis, coord);
    } else {
      // for speed < 0

      speed_half(coord) = neighbor_[axis].next->speed(coord) -
        (1.0f - fabs(gamma)) / 2.0f * neighbor_[axis].next->Limiter(axis, coord);
    }
  }
}


void Cell::ComputeHalfSpeedNextIsBorder(sep::Axis axis, double dt) {

  double numenator1 = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;

  double gamma;
  double p;

  vector<int> coord(3);
  vector<double>::iterator cii;

  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();
        
    if (p > 0.0f) {
      // for speed > 0

      neighbor_[axis].next->speed(coord) = 
        sep::max((double)0.0, 2.0f * speed(coord) - neighbor_[axis].prev->speed(coord));

      speed_half(coord) = speed(coord) +
        (1.0f - fabs(gamma)) / 2.0f * Limiter(axis, coord);

      numenator1 += fabs(p / MolMass()) *
        speed_half(coord);

      numenator2 += fabs(p / MolMass()) *
        0.5f *(speed(coord) + neighbor_[axis].next->speed(coord));
    }
    else {
      // for speed < 0

      denominator += fabs(p / MolMass()) *
              exp((-1.0f) * p * p / (2.0f * MolMass() * wall_t_));
    }

  }


  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();
    double g;

    if (p > 0.0f) {
      // for speed > 0

      neighbor_[axis].prev->speed_half(coord) = neighbor_[axis].prev->speed(coord) +
        (1.0f - fabs(gamma)) / 2.0f * neighbor_[axis].prev->Limiter(axis, coord);
    }
    else {
      // for speed < 0

      speed_half(coord) = 
        numenator1 / denominator * exp((-1.0f * p * p) / (2.0f * MolMass() * wall_t_));

      g = numenator2 / denominator * exp((-1.0f * p * p) / (2.0f * MolMass() * wall_t_));

      neighbor_[axis].next->speed(coord) =
        sep::max((double)0.0, 2.0f * g - speed(coord));

      neighbor_[axis].prev->speed_half(coord) = speed(coord) -
        (1.0f - fabs(gamma)) / 2.0f * Limiter(axis, coord);
    }  
  }

}
