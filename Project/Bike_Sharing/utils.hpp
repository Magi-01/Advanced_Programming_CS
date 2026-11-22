#if !defined UTILS_HPP
#define UTILS_HPP
#include <vector>
#include <ctime>
#include "structs.hpp"
// This function slices a CSVData struct and returns a new CSVData struct containing the specified range of rows.
CSVData slice(const CSVData& d, size_t begin, size_t end);
// Conversion functions for hour to unit (seconds) using sine and cosine transformations.
std::pair<double, double> hour_to_cyclic(size_t r);
// Conversion functions for day to unit (seconds) using sine and cosine transformations.
std::pair<double, double> day_to_cyclic(const std::vector<unsigned int>& day_index_vector, size_t r);
// Utility function to print the headers of the CSV data.
void print_header(const std::vector<std::string>& headers);
// Utility function to print n rows (default 5) of the CSV data.
void print_row(const CSVData& data, size_t n_rows=5);

#endif // UTILS_HPP