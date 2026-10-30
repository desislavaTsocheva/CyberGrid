#include "Battery.hpp"
#include <algorithm>
#include <iostream>


Battery::Battery() : EnergyNode(), currentCharge(0.0), maxCapacity(0.0), efficiency(1.0) {}

Battery::Battery(std::string name, double maxCapacity, double efficiency)
	: EnergyNode(name, maxCapacity), currentCharge(0.0), maxCapacity(maxCapacity), efficiency(efficiency) {
	this->currentCharge = std::min(currentCharge, maxCapacity);
}

double Battery::producePower() const {
	return 0.0;
}

double Battery::getMaxCapacity() const {
	return maxCapacity;
}

double Battery::getCurrentCharge() const {
	return currentCharge;
}

void Battery::update(double step) {

}

void Battery::charge(double amount) {
	if (amount < 0) {
		std::cerr << "Charge amount cannot be negative. Value not set." << std::endl;
		return;
	}

	double energyToAdd = amount * efficiency;
	currentCharge += energyToAdd;
	if (currentCharge > maxCapacity) {
		currentCharge = maxCapacity;
	}
}

double Battery::discharge(double amount) {
	if (amount < 0) {
		std::cerr << "Discharge amount cannot be negative. Value not set." << std::endl;
		return 0.0;
	}
	double energyToDischarge = std::min(amount, currentCharge);
	
	currentCharge -= energyToDischarge;
	
	return energyToDischarge;
}
	