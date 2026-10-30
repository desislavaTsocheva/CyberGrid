#include "DieselGenerator.hpp"
#include <iostream>

DieselGenerator::DieselGenerator() : EnergyNode(), fuelLevel(0.0), fuelConsumptionRate(0.0) {}

DieselGenerator::DieselGenerator(std::string name, double capacity, double fuelLevel, double consumptionRate)
    : EnergyNode(name, capacity), fuelLevel(fuelLevel), fuelConsumptionRate(consumptionRate) {}

double DieselGenerator::producePower() const {
    if (fuelLevel > 0) {
        return capacity;
    }
    return 0.0;
}

void DieselGenerator::update(double step) {
    if (fuelLevel > 0) {
        fuelLevel -= fuelConsumptionRate * step;
        if (fuelLevel < 0) fuelLevel = 0;
    }
}

double DieselGenerator::getFuelLevel() const {
    return fuelLevel;
}

void DieselGenerator::refuel(double amount) {
    if (amount > 0) {
        fuelLevel += amount;
    }
}