#include "test.h"

using namespace std;

void NotchTheTime(string process_name) {
  static bool running = false;
  static clock_t start_computing;
  static string p_name = process_name;

  if (!running) {
    running = true;
    start_computing = clock();
    p_name = process_name;
  }
  else {
    running = false;
    double time = float(clock () - start_computing) /  CLOCKS_PER_SEC;
    time *= 7000; // time for all cells
    cout << "Taken time for " << p_name << ": " <<
      (int)(time/60.0) << " m " <<
      time - (int)(time/60.0)*60 << " s." << endl;
  }
}
