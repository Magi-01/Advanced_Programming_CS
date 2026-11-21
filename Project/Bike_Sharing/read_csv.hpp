#if !defined READ_CSV_HPP
#define READ_CSV_HPP
#include "structs.hpp"
// This function reads a CSV file and returns a CSVData struct containing the data.
CSVData read_csv(const std::string& filename, const std::vector<std::string>& expected_headers);
// This function slices a CSVData struct and returns a new CSVData struct containing the specified range of rows.
CSVData slice(const CSVData& d, size_t begin, size_t end);

#endif // READ_CSV_HPP