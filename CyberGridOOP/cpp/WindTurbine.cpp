#include "WindTurbine.hpp"
#include <iostream>

WindTurbine::WindTurbine() : EnergyNode(), windSpeed(0.0) {}

WindTurbine::WindTurbine(std::string name, double capacity, double windSpeed)
    : EnergyNode(name, capacity), windSpeed(windSpeed) {}

double WindTurbine::producePower() const {
    double efficiencyFactor = windSpeed / 20.0;
    if (efficiencyFactor > 1.0) efficiencyFactor = 1.0;
    return capacity * efficiencyFactor;
}

void WindTurbine::update(double step) {
    windSpeed += 1.5 * step;
    if (windSpeed > 25.0) {
        windSpeed = 5.0;
    }
}

double WindTurbine::getWindSpeed() const {
    return windSpeed;
}

void WindTurbine::setWindSpeed(double speed) {
    if (speed >= 0.0) {
        this->windSpeed = speed;
    } else {
        std::cerr << "Wind speed cannot be negative.\n";
    }
}