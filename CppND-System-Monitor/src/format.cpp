#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int sec = 0;
    int min = 0;
    int h = 0;
    sec = seconds % 60;
    min = (seconds / 60) % 60;
    h = seconds / 60 / 60;

    return std::to_string(h) + ":" + std::to_string(min) + ":" + std::to_string(sec);
}