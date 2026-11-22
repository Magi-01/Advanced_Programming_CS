#include <vector>
#include <ctime>
#include "structs.hpp"
#include <cmath>
#include "utils.hpp"

constexpr double PI = 3.14159265358979323846;
constexpr size_t N_ROWS = 1416;

bool validate_timestamps(const std::vector<std::tm>& ts) {
    if (ts.size() != 1416) return false;

    for (size_t r = 0; r < ts.size(); ++r) {
        std::tm exp{};
        exp.tm_year = 2026+1900;
        exp.tm_mon  = 0;            // January
        exp.tm_mday = 1;
        exp.tm_hour = static_cast<int>(r);   // mktime normalizes overflow
        exp.tm_isdst = -1;
        std::mktime(&exp);          // normalizes to the right date/hour

        if (ts[r].tm_year != exp.tm_year || ts[r].tm_mon  != exp.tm_mon  ||
            ts[r].tm_mday != exp.tm_mday || ts[r].tm_hour != exp.tm_hour ||
            ts[r].tm_min  != 0           || ts[r].tm_sec  != 0)
            return false;
    }
    return true;
}

ValidationError validate_hour_cos_sin(const std::vector<double>& hour_sin,
                                       const std::vector<double>& hour_cos) {
    if (hour_sin.size() != N_ROWS || hour_cos.size() != N_ROWS)
        return ValidationError::fail("Invalid vector sizes", 0);

    for (size_t r = 0; r < N_ROWS; ++r) {

        const auto [exp_sin, exp_cos] = hour_to_cyclic(r);

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

        const auto [exp_sin, exp_cos] = day_to_cyclic(day_index, r);

        if (std::abs(day_sin[r] - exp_sin) > 1e-9 || std::abs(day_cos[r] - exp_cos) > 1e-9) {
            std::ostringstream os;
            os << "Invalid day sin/cos: got (" << day_sin[r] << ", " << day_cos[r]
               << "), expected (" << exp_sin << ", " << exp_cos << ")";
            return ValidationError::fail(os.str(), r);
        }
    }
    return ValidationError::success();
}