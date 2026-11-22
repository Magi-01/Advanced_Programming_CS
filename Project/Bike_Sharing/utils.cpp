#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include "structs.hpp"

constexpr double PI = 3.14159265358979323846; // Define a constant for pi

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

std::pair<double, double> hour_to_cyclic(size_t r) {
    // Convert hours to seconds
    const unsigned int h = static_cast<unsigned int>(r % 24);
    const double angle =  2 * PI * h / 24; // Calculate angle in radians for the hour
    return {std::sin(angle), std::cos(angle)}; // Return the sine and cosine values for the hour
}

std::pair<double, double> day_to_cyclic(const std::vector<unsigned int>& day_index_vector, size_t r) {
    const unsigned int dow = (day_index_vector[r] + 3) % 7;
    const double angle = 2.0 * PI * dow / 7.0; // Calculate angle in radians for the day of the week
    return {std::sin(angle), std::cos(angle)}; // Return the sine and cosine values for the day of the week
}

void print_header(const std::vector<std::string>& headers) {
    std::cout << "\nHeaders: ";
    for (const auto& header : headers) {
        std::cout << header << " | ";
    }
    std::cout << std::endl;
}

void print_row(const CSVData& data, size_t n_rows=5) {
    if (data.rows() > 0) {
    std::cout << "\nFirst " << n_rows << " rows: " << std::endl;
    for (size_t i = 0; i < std::min(n_rows, data.rows()); ++i) {
        std::cout << 1900 + data.timestamp[i].tm_year << "-" << 1 + data.timestamp[i].tm_mon << "-" << data.timestamp[i].tm_mday << " "
                  << data.count[i] << " "
                  << data.hour_sin[i] << " "
                  << data.hour_cos[i] << " "
                  << data.day_sin[i] << " "
                  << data.day_cos[i] << " "
                  << static_cast<int>(data.is_weekend[i]) << " "
                  << data.day_index[i] << " "
                  << static_cast<int>(data.is_february[i]) << " "
                  << static_cast<int>(data.is_holiday[i])  << "\n" 
                  << std::endl;
                }
    } else {
        std::cout << "No data available." << std::endl;
    }
}