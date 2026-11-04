#include <iostream>
#include <stdlib.h>

double conversion(double value, char unit) {
    if (unit == 'C' || unit == 'c') {
        return (value * 9.0 / 5.0) + 32; // Celsius to Fahrenheit
    } else if (unit == 'F' || unit == 'f') {
        return (value - 32) * 5.0 / 9.0; // Fahrenheit to Celsius
    } else {
        throw std::invalid_argument("Invalid unit. Use 'C' for Celsius or 'F' for Fahrenheit.");
    }
}

void print_conversion(double value, char unit) {
    try {
        double converted_value = conversion(value, unit);
        if (unit == 'C' || unit == 'c') {
            std::cout << value << "°C is " << converted_value << "°F" << std::endl;
        } else {
            std::cout << value << "°F is " << converted_value << "°C" << std::endl;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    }
}

int main() {
    double value;
    char unit;

    std::cout << "Enter the temperature value: ";
    std::cin >> value;
    std::cout << "Enter the unit (C for Celsius, F for Fahrenheit): ";
    std::cin >> unit;

    print_conversion(value, unit);

    return 0;
}