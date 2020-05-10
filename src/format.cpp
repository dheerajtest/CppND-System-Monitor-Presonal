#include <string>
#include <iomanip>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int H = seconds / 3600;
  int S = seconds % 3600;
  int M = S / 60;
  S = S % 60;
  std::ostringstream time;
  time << std::setfill('0') << std::setw(2) << H << ":";
  time << std::setfill('0') << std::setw(2) << M << ":";
  time << std::setfill('0') << std::setw(2) << S;
  return time.str();
 }