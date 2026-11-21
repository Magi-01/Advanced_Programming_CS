#if !defined VALIDATION_HPP
#define VALIDATION_HPP
#include <vector>
#include <ctime>
#include "structs.hpp"
// Validate timestamps to ensure they are in the expected format and range.
bool validate_timestamps(const std::vector<std::tm>& ts);
// Validate hour sine and cosine values to ensure they are in the expected range.
ValidationError validate_hour_cos_sin(const std::vector<double>& hour_sin, const std::vector<double>& hour_cos);
// Validate day sine and cosine values to ensure they are in the expected range.
ValidationError validate_day_cos_sin(const std::vector<double>& day_sin, const std::vector<double>& day_cos, const std::vector<unsigned int>& day_index);

#endif // VALIDATION_HPP