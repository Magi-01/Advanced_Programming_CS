#if !defined READ_CSV_HPP
#define READ_CSV_HPP
#include "structs.hpp"
// This function reads a CSV file and returns a CSVData struct containing the data.
CSVData read_csv(const std::string& filename, const std::vector<std::string>& expected_headers);

#endif // READ_CSV_HPP