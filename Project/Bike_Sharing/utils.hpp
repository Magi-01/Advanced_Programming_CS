#if !defined UTILS_HPP
#define UTILS_HPP
#include <vector>
#include <ctime>
#include "structs.hpp"
// This function slices a CSVData struct and returns a new CSVData struct containing the specified range of rows.
CSVData slice(const CSVData& d, size_t begin, size_t end);
// Maps an hour of day (0..23) to (sin, cos) of 2*pi*hour/24.
std::pair<double, double> hour_to_cyclic(unsigned int hour);
// Maps a day of week (0..6, Monday = 0) to (sin, cos) of 2*pi*dow/7.
std::pair<double, double> day_to_cyclic(unsigned int dow);
// Day of week (Monday = 0) for a day index counted from 2026-01-01, which is a Thursday.
unsigned int day_of_week(size_t day_index);
// Utility function to print the headers of the CSV data.
void print_header(const std::vector<std::string>& headers);
// Utility function to print n rows (default 5) of the CSV data.
void print_row(const CSVData& data, size_t n_rows=5);

#endif // UTILS_HPP