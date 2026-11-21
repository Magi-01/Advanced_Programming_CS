#include <iostream>
#include "stdlib.h"
#include <ctime>
#include <vector>

constexpr double PI = 3.14159265358979323846; // Define a constant for pi

double sin(double x) {
    // sine function implementation using Taylor series expansion
    double term = x; // First term of the series
    double sum = term; // Initialize sum of series
    for (int n = 1; n < 10; ++n) { // Calculate next term in series
        term *= -x * x / ((2 * n) * (2 * n + 1)); // Update term using recurrence relation
        sum += term;
    }
    return sum;
}

double cos(double x) {
    // cosine function implementation using Taylor series expansion
    double term = 1; // First term of the series
    double sum = term; // Initialize sum of series
    for (int n = 1; n < 10; ++n) { // Calculate next term in series
        term *= -x * x / ((2 * n - 1) * (2 * n)); // Update term using recurrence relation
        sum += term;
    }
    return sum;
}

std::pair<double, double> hour_to_unit(unsigned int hour) {
    // Convert hours to seconds
    const double angle =  2 * PI * hour / 24; // Calculate angle in radians for the hour
    return {sin(angle), cos(angle)}; // Return the sine and cosine values for the hour
}

std::pair<double, double> day_to_cyclic(unsigned int day) {
    const double angle = 2.0 * PI * (day % 7) / 7.0; // Calculate angle in radians for the day of the week
    return {sin(angle), cos(angle)}; // Return the sine and cosine values for the day of the week
}