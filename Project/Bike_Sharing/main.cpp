#include <iostream>
#include "utils.hpp"
#include "read_csv.hpp"
#include <ctime>
#include "validation.hpp"
#include "structs.hpp"
#include <cstdint>

int main() {
    const std::vector<std::string> expected = {
        "timestamp", "count", "hour_sin", "hour_cos", "dow_sin", "dow_cos",
        "is_weekend", "day_index", "is_february", "is_holiday"
    };

    try {

        CSVData data = read_csv("citibike_hourly_2026_01_02.csv", expected);

        print_header(data.headers); // Print the headers of the CSV data
        print_row(data, 5); // Print the first 5 rows of the CSV data

        auto ts_res   = validate_timestamps(data.timestamp);
        auto hour_res = validate_hour_cos_sin(data.hour_sin, data.hour_cos);
        auto day_res  = validate_day_cos_sin(data.day_sin, data.day_cos, data.day_index);
        auto cal_res = validate_calendar_features(data.day_index, data.is_weekend,
                                          data.is_february, data.is_holiday);

        bool ok = true;
        for (const auto& [name, res] : {std::pair{"Timestamp", ts_res},
                                        std::pair{"Hour",      hour_res},
                                        std::pair{"Day",       day_res},
                                        std::pair{"Cal",       cal_res}}) {
            if (!res.ok()) {
                std::cerr << name << " check, row " << res.row << ": " << res.message << '\n';
                ok = false;
            }
        }
        if (!ok) return 1;
        
        constexpr size_t TRAIN_N = 1080, VAL_N = 168;

        // Split the data into training, validation, and test sets
        CSVData training_data   = slice(data, 0,                 TRAIN_N);
        CSVData validation_data = slice(data, TRAIN_N,           TRAIN_N + VAL_N);
        CSVData test_data       = slice(data, TRAIN_N + VAL_N,   data.rows());

        // first and last values for training, validation, and test sets
        std::cout << "Training set: " << training_data.rows() << " rows, first timestamp: "
                    << training_data.timestamp.front().tm_year + 1900 << "-" << training_data.timestamp.front().tm_mon + 1 << "-" << training_data.timestamp.front().tm_mday
                    << ", last timestamp: "
                    << training_data.timestamp.back().tm_year + 1900 << "-" << training_data.timestamp.back().tm_mon + 1 << "-" << training_data.timestamp.back().tm_mday
                    << std::endl;

        std::cout << "Validation set: " << validation_data.rows() << " rows, first timestamp: "
                    << validation_data.timestamp.front().tm_year + 1900 << "-" << validation_data.timestamp.front().tm_mon + 1 << "-" << validation_data.timestamp.front().tm_mday
                    << ", last timestamp: "
                    << validation_data.timestamp.back().tm_year + 1900 << "-" << validation_data.timestamp.back().tm_mon + 1 << "-" << validation_data.timestamp.back().tm_mday
                    << std::endl;

        std::cout << "Test set: " << test_data.rows() << " rows, first timestamp: "
                    << test_data.timestamp.front().tm_year + 1900 << "-" << test_data.timestamp.front().tm_mon + 1 << "-" << test_data.timestamp.front().tm_mday
                    << ", last timestamp: "
                    << test_data.timestamp.back().tm_year + 1900 << "-" << test_data.timestamp.back().tm_mon + 1 << "-" << test_data.timestamp.back().tm_mday
                    << std::endl;
    } catch (const ValidationError& e){
        std::cerr << "Error (line " << e.row << "): " << e.message << '\n';
        return 1;
    } catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}