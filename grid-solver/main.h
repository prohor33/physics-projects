#ifndef _MAIN_H_
#define _MAIN_H_

#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h> // for nan

#include <mpi.h>

using namespace std;

namespace sep { // my separator namespace

  enum CellType {
    NONE = 0,
    NORMAL = 1,
    FAKE = 2,
  };

  enum OutType {
    OUT_BINARY_FILE,
    OUT_TXT_FILE
  };

  enum Parameter {
    T = 0,
    n = 1
  };

  enum SpeedType {
    Speed = 0,
    HalfSpeed = 1
  };

  enum {
    INDEX = 3
  };

  enum Axis {
    X = 0,
    Y = 1,
    Z = 2
  };

  enum Limiter {
    SUPERBEE = 0,
    MC = 1,
    STRANGE = 2
  };

  enum GasNumb {
    FIRST = 0,
    SECOND = 1
  };

  template <typename T> inline T sqr(T x) { return x*x; }
  template <typename T> inline T sign(T x) { return x == 0 ? 0 : (x > 0 ? 1 : -1); };
  template <typename T> inline T module(T x) { return x * sign(x); }
  template <typename T> inline T min(T x, T y) { return x > y ? y : x; };
  template <typename T> inline T max(T x, T y) { return x > y ? x : y; };

  string int_to_string(int i);
}

#endif // _MAIN_H_
