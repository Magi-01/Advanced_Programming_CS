#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include "structs.hpp"
#include <cstdint>
#include "constants.hpp"

// This function splits a string into a vector of strings using the specified delimiter (default is comma).
static std::vector<std::string> split(const std::string& line, char delim = ',') {
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    while (std::getline(ss, field, delim)) fields.push_back(field);
    return fields;
}

CSVData read_csv(const std::string& filename, const std::vector<std::string>& expected_headers) {
    CSVData data;
    std::ifstream file(filename);
    // Check if the file was opened successfully
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return data; // empty, but valid
    }

    std::string line;
    // Read the header line
    if (!std::getline(file, line)) 
        throw ValidationError::fail("File is empty: " + filename, 0);;
    // Remove any trailing carriage return character from the line (for Windows compatibility)
    if (!line.empty() && line.back() == '\r') line.pop_back();
    data.headers = split(line);

    // Check if the headers match the expected headers
    if (data.headers != expected_headers) {
        std::cerr << "Header mismatch.\nExpected:";
        for (const auto& e : expected_headers) std::cerr << ' ' << e;
        std::cerr << "\nGot:     ";
        for (const auto& h : data.headers) std::cerr << ' ' << h;
        std::cerr << '\n';
        ValidationError mis = ValidationError::fail("Header mismatch", 0);
        throw mis;
    }

    constexpr size_t NCOLS = 10; // Number of expected columns in the CSV file
    size_t line_no = 1; // Initialize line number to 1 (header line)

    // Read each line of the CSV file and parse the data into the CSVData struct
    while (std::getline(file, line)) {
        ++line_no;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        // Split the line into fields using the split function
        auto f = split(line);
        // Check if the number of fields matches the expected number of columns
        if (f.size() != NCOLS) {
            std::cerr << "Line " << line_no << ": expected " << NCOLS
                      << " fields, got " << f.size() << ", skipping\n";
            continue;
        }

        try {
            std::tm ts{};
            if (!parse_timestamp(f[0], ts)) {
                std::cerr << "Line " << line_no << ": invalid timestamp '" << f[0] << "', skipping\n";
                continue;
            }

            const unsigned int cnt = static_cast<unsigned int>(std::stoul(f[1]));
            const unsigned int di  = static_cast<unsigned int>(std::stoul(f[7]));

            const double hs = std::stod(f[2]), hc = std::stod(f[3]);
            const double ds = std::stod(f[4]), dc = std::stod(f[5]);

            auto flag = [&](const std::string& s, const char* name) -> uint8_t {
                const int v = std::stoi(s);
                if (v != 0 && v != 1)
                    throw std::invalid_argument(std::string(name) + " must be 0 or 1, got " + s);
                return static_cast<uint8_t>(v);
            };
            const uint8_t wk  = flag(f[6], "is_weekend");
            const uint8_t fb  = flag(f[8], "is_february");
            const uint8_t hol = flag(f[9], "is_holiday");

            data.timestamp.push_back(ts);
            data.count.push_back(cnt);
            data.hour_sin.push_back(hs);  data.hour_cos.push_back(hc);
            data.day_sin.push_back(ds);   data.day_cos.push_back(dc);
            data.is_weekend.push_back(wk);
            data.day_index.push_back(di);
            data.is_february.push_back(fb);
            data.is_holiday.push_back(hol);
        } catch (const std::exception& e) {
            throw ValidationError::fail(std::string("Parse error: ") + e.what(), line_no);
        }
    }

    if (data.rows() != N_ROWS) {
        throw ValidationError::fail(
            "Expected " + std::to_string(N_ROWS) + " rows, got " + std::to_string(data.rows()),
            line_no);
    } else {
        std::cout << "Successfully read " << data.rows() << " rows from file: " << filename << std::endl;
    }

    if (!file.eof()) {
        throw ValidationError::fail("Error reading file, got no end of file on line: ", line_no);
    } else {
        std::cout << "Finished reading file: " << filename << std::endl;
    }

    return data;
}