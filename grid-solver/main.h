#ifndef _MAIN_H_
#define _MAIN_H_

#include <vector>
#include <math.h>
#include <iostream>

using namespace std;

namespace sep { // my separator namespace

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

  template <typename T> inline T sqr(T x) { return x*x; }
  template <typename T> inline T min(T x) { return x*x; }
  template <typename T> inline T sign(T x) { return x == 0 ? 0 : (x > 0 ? 1 : -1); };
  template <typename T> inline T module( T x ) { return x * sign( x ); }
  template <typename T> inline T min( T x, T y ) { return x > y ? y : x; };
  template <typename T> inline T max( T x, T y ) { return x > y ? x : y; };

}

#endif // _MAIN_H_
