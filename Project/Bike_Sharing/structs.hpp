#if !defined STRUCTS_HPP
#define STRUCTS_HPP
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cmath>
#include <cstddef>
#include <stdexcept>

// This struct is used to hold the data read from a CSV file. Each member variable is a vector that holds one column of data.
// The headers vector holds the names of the columns, and the other vectors hold the data for each column.
// The rows() function returns the number of rows in the data, which is equal to the size of the timestamp vector (or any other vector, since they should all be the same size).
struct CSVData {
    std::vector<std::string> headers; // column names
    std::vector<std::tm> timestamp;   // "YYYY-MM-DD hh:mm:ss"
    std::vector<unsigned int>    count, day_index; // count of bikes, day index (0=Jan 1, 1=Jan 2, ...)
    std::vector<double> hour_sin, hour_cos, day_sin, day_cos; // sine and cosine transformations of hour and day of week
    std::vector<uint8_t>    is_weekend , is_february, is_holiday; // boolean flags for weekend, February, and holiday

    size_t rows() const { return timestamp.size(); } // Returns the number of rows in the data, which is equal to the size of the timestamp vector.
};

// This function parses a timestamp string in the format "YYYY-MM-DD hh:mm:ss" into a std::tm struct.
inline bool parse_timestamp(const std::string& s, std::tm& out) {
    std::istringstream ss(s); // Create an input string stream from the timestamp string
    ss >> std::get_time(&out, "%Y-%m-%d %H:%M:%S"); // Parse the timestamp string into the std::tm struct using the specified format
    return !ss.fail();
}

// This struct is used to hold an error message and the line number where the error occurred.
// It is used in the validation functions to return an error message and the line number of the first error encountered.
struct ValidationError {
    std::string message;   // empty means success
    size_t row = 0; // line number of the first error encountered

    bool ok() const { return message.empty(); } // Returns true if there is no error message, indicating success.
    static ValidationError success() { return {}; } // Returns a ValidationError object indicating success (no error).
    static ValidationError fail(std::string msg, size_t r) { return {std::move(msg), r}; } // Returns a ValidationError object indicating failure, with the provided error message and line number.
};
#endif // STRUCTS_HPP