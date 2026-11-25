#include "structs.hpp"
#include "utils.hpp"
#include "constants.hpp"
#include <cmath>
#include <vector>
#include <ctime>
#include <array>
#include <algorithm>
#include <string>




ValidationError validate_timestamps(const std::vector<std::tm>& ts) {
    if (ts.size() != N_ROWS)
        return ValidationError::fail("Invalid vector sizes", 0);

    for (size_t r = 0; r < ts.size(); ++r) {
        std::tm expected{};
        expected.tm_year  = 2026 - 1900;
        expected.tm_mon   = 0;
        expected.tm_mday  = 1;
        expected.tm_hour  = static_cast<int>(r);   // mktime normalizes the overflow
        expected.tm_isdst = -1;
        std::mktime(&expected);

        if (ts[r].tm_year != expected.tm_year || ts[r].tm_mon  != expected.tm_mon  ||
            ts[r].tm_mday != expected.tm_mday || ts[r].tm_hour != expected.tm_hour ||
            ts[r].tm_min  != 0                || ts[r].tm_sec  != 0)
            return ValidationError::fail("Timestamp is not the expected hourly sequence", r);
    }
    return ValidationError::success();
}

ValidationError validate_hour_cos_sin(const std::vector<double>& hour_sin,
                                       const std::vector<double>& hour_cos) {
    if (hour_sin.size() != N_ROWS || hour_cos.size() != N_ROWS)
        return ValidationError::fail("Invalid vector sizes", 0);

    for (size_t r = 0; r < N_ROWS; ++r) {

        const auto [exp_sin, exp_cos] = hour_to_cyclic(static_cast<unsigned int>(r % 24));

        if (std::abs(hour_sin[r] - exp_sin) > 1e-9 ||
            std::abs(hour_cos[r] - exp_cos) > 1e-9) {
            std::ostringstream os;
            os << "Invalid hour sin/cos: got (" << hour_sin[r] << ", " << hour_cos[r]
               << "), expected (" << exp_sin << ", " << exp_cos << ")";
            return ValidationError::fail(os.str(), r);
        }
    }
    return ValidationError::success();
}

ValidationError validate_day_cos_sin(const std::vector<double>& day_sin, const std::vector<double>& day_cos, const std::vector<unsigned int>& day_index) {
    if (day_sin.size() != N_ROWS || day_cos.size() != N_ROWS || day_index.size() != N_ROWS)
        return ValidationError::fail("Invalid vector sizes", 0);

    for (size_t r = 0; r < N_ROWS; ++r) {

        const auto [exp_sin, exp_cos] = day_to_cyclic(day_of_week(day_index[r]));

        if (std::abs(day_sin[r] - exp_sin) > 1e-9 || std::abs(day_cos[r] - exp_cos) > 1e-9) {
            std::ostringstream os;
            os << "Invalid day sin/cos: got (" << day_sin[r] << ", " << day_cos[r]
               << "), expected (" << exp_sin << ", " << exp_cos << ")";
            return ValidationError::fail(os.str(), r);
        }
    }
    return ValidationError::success();
}

ValidationError validate_calendar_features(const std::vector<unsigned int>& day_index,
                                           const std::vector<uint8_t>& is_weekend,
                                           const std::vector<uint8_t>& is_february,
                                           const std::vector<uint8_t>& is_holiday) {
    if (day_index.size() != N_ROWS || is_weekend.size() != N_ROWS ||
        is_february.size() != N_ROWS || is_holiday.size() != N_ROWS)
        return ValidationError::fail("Invalid vector sizes", 0);

    // Day indices of the holidays in the file: Jan 1, Jan 19, Feb 16
    static constexpr std::array<size_t, 3> HOLIDAYS = {0, 18, 46};

    for (size_t r = 0; r < N_ROWS; ++r) {
        const size_t idx = r / 24;
        const size_t dow = day_of_week(idx);

        const uint8_t exp_weekend  = dow >= 5 ? 1 : 0;
        const uint8_t exp_february = idx >= 31 ? 1 : 0;
        const uint8_t exp_holiday  =
            std::find(HOLIDAYS.begin(), HOLIDAYS.end(), idx) != HOLIDAYS.end() ? 1 : 0;

        auto bad = [&](const char* name, unsigned got, unsigned want) {
            return ValidationError::fail(
                std::string(name) + ": got " + std::to_string(got) +
                ", expected " + std::to_string(want), r);
        };

        if (day_index[r]   != idx)          return bad("day_index",   day_index[r],   static_cast<unsigned>(idx));
        if (is_weekend[r]  != exp_weekend)  return bad("is_weekend",  is_weekend[r],  exp_weekend);
        if (is_february[r] != exp_february) return bad("is_february", is_february[r], exp_february);
        if (is_holiday[r]  != exp_holiday)  return bad("is_holiday",  is_holiday[r],  exp_holiday);
    }
    return ValidationError::success();
}