#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include "structs.hpp"
#include <cstdint>

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
    if (!std::getline(file, line)) return data;
    if (!line.empty() && line.back() == '\r') line.pop_back();
    data.headers = split(line);

    if (data.headers != expected_headers) {
        std::cerr << "Header mismatch.\nExpected:";
        for (const auto& e : expected_headers) std::cerr << ' ' << e;
        std::cerr << "\nGot:     ";
        for (const auto& h : data.headers) std::cerr << ' ' << h;
        std::cerr << '\n';
        ValidationError mis = ValidationError::fail("Header mismatch", 0);
        throw mis;
    }

    constexpr size_t NCOLS = 10;
    size_t line_no = 1;

    while (std::getline(file, line)) {
        ++line_no;
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;

        auto f = split(line);
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

    if (data.rows() < 1416) {
        ValidationError not_enough_data = ValidationError::fail("Not enough data read from file", line_no);
        throw not_enough_data;
    } else {
        std::cout << "Successfully read " << data.rows() << " rows from file: " << filename << std::endl;
    }

    if (!file.eof()) {
        ValidationError fileread_error = ValidationError::fail("Error reading file", line_no);
        throw fileread_error;
    } else {
        std::cout << "Finished reading file: " << filename << std::endl;
    }

    file.close(); // file close
    return data;
}

CSVData slice(const CSVData& d, size_t begin, size_t end) {
        if (begin > end || end > d.rows())
            throw std::out_of_range("slice: invalid range");

        CSVData out;
        out.headers = d.headers;

        auto cut = [&](auto& dst, const auto& src) {
            dst.assign(src.begin() + begin, src.begin() + end);
        };

        cut(out.timestamp,   d.timestamp);
        cut(out.count,       d.count);
        cut(out.day_index,   d.day_index);
        cut(out.hour_sin,    d.hour_sin);
        cut(out.hour_cos,    d.hour_cos);
        cut(out.day_sin,     d.day_sin);
        cut(out.day_cos,     d.day_cos);
        cut(out.is_weekend,  d.is_weekend);
        cut(out.is_february, d.is_february);
        cut(out.is_holiday,  d.is_holiday);
        return out;
    };