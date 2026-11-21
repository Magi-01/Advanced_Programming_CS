#if !defined UTILS_HPP
#define UTILS_HPP
#include <vector>
#include <ctime>
// Conversion functions for hour to unit (seconds) using sine and cosine transformations.
std::pair<double, double> hour_to_unit(unsigned int hour);
// Conversion functions for day to unit (seconds) using sine and cosine transformations.
std::pair<double, double> day_to_cyclic(unsigned int day);

#endif // UTILS_HPP