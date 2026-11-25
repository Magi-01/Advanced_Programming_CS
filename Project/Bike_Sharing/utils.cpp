#include "utils.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <ctime>
#include "structs.hpp"
#include "constants.hpp"

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

std::pair<double, double> hour_to_cyclic(unsigned int hour) {
    const double angle = 2.0 * PI * hour / 24.0;
    return {std::sin(angle), std::cos(angle)};
}

std::pair<double, double> day_to_cyclic(unsigned int dow) {
    const double angle = 2.0 * PI * dow / 7.0;
    return {std::sin(angle), std::cos(angle)};
}

unsigned int day_of_week(size_t day_index) {
    return static_cast<unsigned int>((day_index + 3) % 7);
}

void print_header(const std::vector<std::string>& headers) {
    std::cout << "\nHeaders: ";
    for (const auto& header : headers) {
        std::cout << header << " | ";
    }
    std::cout << std::endl;
}

void print_row(const CSVData& data, size_t n_rows) {
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