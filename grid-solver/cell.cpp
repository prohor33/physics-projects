#include "cell.h"
#include "parameters.h"

#include <stdlib.h> // for exit()

CellNeighbor::CellNeighbor(
    Cell* next_c,
    Cell* prev_c
    ) :
    next(next_c),
    prev(prev_c)
{
  // some cheat, but we strongly need it
  if (next && next->type() == Cell::NONE)
    next = NULL;

  if (prev && prev->type() == Cell::NONE)
    prev = NULL;
};

Cell::Cell(sep::GasNumb gas_numb, CellType type) :
    gas_numb_(gas_numb),
    type_(type)
  {

  wall_t_ = vector<double>(3);

  vector<int> coord;

  if (type_ == NONE)  // empty cell
    return;

  double n = 1.0;
  double T_start = 1.0;
  double C = 0;
  double p2;

  // process C
  for (int i=0; i<PARAMETERS->s_coord_map_1d_to_3d_.size(); i++) {

    coord = PARAMETERS->s_coord_map_1d_to_3d_[i];
    p2 = P2(coord);

    C += exp((-1.0f)*
        p2 / (2.0f * MolMass() * T_start));
  }

  /* n0 = 1.0 */
  C = 1.0f / C;

  for (int i=0; i<PARAMETERS->s_coord_map_1d_to_3d_.size(); i++) {

    coord = PARAMETERS->s_coord_map_1d_to_3d_[i];
    p2 = P2(coord);

    speed_.push_back(n * C * exp((-1.0f)*
        p2 / (2.0f * MolMass() * T_start)));
  }

  // initialize speed_half array
  speed_half_ = vector<double>(PARAMETERS->s_coord_map_1d_to_3d_.size());
};

inline double Cell::MolMass() const {
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

inline double Cell::P(sep::Axis axis,
    vector<int> coord) const {

  return PARAMETERS->velocity_[coord[axis]] * MolMass();
}

double Cell::P2(vector<int> coord) const {

  return (sep::sqr(PARAMETERS->velocity_[coord[sep::X]]) +
      sep::sqr(PARAMETERS->velocity_[coord[sep::Y]]) +
      sep::sqr(PARAMETERS->velocity_[coord[sep::Z]])) *
      sep::sqr(MolMass());
}

double Cell::Limiter(sep::Axis axis,
    vector<int> coord) {

  if( !neighbor_[axis].next || !neighbor_[axis].prev ) {
    cout << "error limiter" << endl;
    return -1.0f;
  }

  double a;

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

inline vector<int> Cell::GetSpeedCoord(int index) {

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

  if (type_ != NORMAL) {
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

  if (type_ != NORMAL) {
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

    speed(coord) +=
        - gamma * (speed_half(coord) - neighbor_[axis].prev->speed_half(coord));
  }
}

void Cell::ComputeHalfSpeedPrevIsBorder(sep::Axis axis, double dt) {

  double numenator1 = 0.0f;
  double numenator2 = 0.0f;
  double denominator = 0.0f;

  double gamma;
  double p, p2;

  vector<int> coord(3);
  vector<double>::iterator cii;

  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    p2 = P2(coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();

    // for speed < 0 with (18.5) compute prev->speed()

    if (p > 0.0f) {
      // for speed > 0

      denominator += fabs(p / MolMass()) *
        exp((-1.0f) * p2 / (2.0f * MolMass() * wall_t_[axis]));
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
    p2 = P2(coord);
    gamma = dt * p / MolMass() * PARAMETERS->time_step_ / H();
    double g;

    if (p > 0.0f) {
      // for speed > 0

      neighbor_[axis].prev->speed_half(coord) =
         numenator1 / denominator * exp((-1.0f) * p2 / (2.0f * MolMass() * wall_t_[axis]));

      g = numenator2 / denominator * exp((-1.0f) * p2 / (2.0f * MolMass() * wall_t_[axis]));

      neighbor_[axis].prev->speed(coord) =
        sep::max((double)0.0, 2.0f * g - speed(coord));
//      cout << "2.0f * g - speed(coord) = " << 2.0f * g - speed(coord) << endl;
//      cout << "2.0f * MolMass() * wall_t_ = " << 2.0f * MolMass() * wall_t_ << endl;
//      cout << "sep::sqr(p) = " << sep::sqr(p) << endl;
//      cout << "pow = " << (-1.0f) * sep::sqr(p) / (2.0f * MolMass() * wall_t_) << endl;
//      cout << "exp = " << exp((-1.0f) * sep::sqr(p) / (2.0f * MolMass() * wall_t_)) << endl;
//      cout << "g = " << g << endl;
//      cout << "speed(coord) = " << speed(coord) << endl;
      speed_half(coord) = speed(coord) +
        (1.0f - fabs(gamma)) / 2.0f * Limiter(axis, coord);

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
  double p, p2;

  vector<int> coord(3);
  vector<double>::iterator cii;

  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    p2 = P2(coord);
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
              exp((-1.0f) * p2 / (2.0f * MolMass() * wall_t_[axis]));
    }

  }


  for (cii=speed_half_.begin(); cii!=speed_half_.end(); ++cii) {

    coord = GetSpeedCoord((int)(cii-speed_half_.begin()));
    p = P(axis, coord);
    p2 = P2(coord);
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
        numenator1 / denominator * exp((-1.0f * p2) / (2.0f * MolMass() * wall_t_[axis]));

      g = numenator2 / denominator * exp((-1.0f * p2) / (2.0f * MolMass() * wall_t_[axis]));

      neighbor_[axis].next->speed(coord) =
        sep::max((double)0.0, 2.0f * g - speed(coord));

      neighbor_[axis].prev->speed_half(coord) = speed(coord) -
        (1.0f - fabs(gamma)) / 2.0f * Limiter(axis, coord);
    }  
  }

}
