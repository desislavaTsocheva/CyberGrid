#include "Battery.hpp"
#include <algorithm>

Battery::Battery() : EnergyNode(), currentCharge(0.0), maxCapacity(0.0), efficiency(1.0) {}

Battery::Battery(std::string name, double maxCapacity, double efficiency)
    : EnergyNode(name, maxCapacity), currentCharge(0.0), maxCapacity(maxCapacity), efficiency(efficiency) {}

double Battery::producePower() const {
    return 0.0;
}

double Battery::getMaxCapacity() const {
    return maxCapacity;
}

double Battery::getCurrentCharge() const {
    return currentCharge;
}

double Battery::getEfficiency() const {
    return efficiency;
}

void Battery::update(double step) {
    (void)step;
}

void Battery::charge(double amount) {
    if (amount < 0) return;
    double energyToAdd = amount * efficiency;
    currentCharge += energyToAdd;
    if (currentCharge > maxCapacity) {
        currentCharge = maxCapacity;
    }
}

double Battery::discharge(double amount) {
    if (amount < 0) return 0.0;
    double energyToDischarge = std::min(amount, currentCharge);
    currentCharge -= energyToDischarge;
    return energyToDischarge;
}