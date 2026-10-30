#include "Consumer.hpp"
#include <iostream>


Consumer::Consumer()
    : EnergyNode("Default_Consumer", 0.0), consumptionRate(0.0) {}

Consumer::Consumer(const std::string& name, double maxCapacity, double consumptionRate)
    : EnergyNode(name, maxCapacity), consumptionRate(consumptionRate) {}

double Consumer::producePower() const {
    return 0.0;
}

void Consumer::update(double timeStep) {

}

double Consumer::getConsumptionRate() const {
    return consumptionRate;
}

void Consumer::setConsumptionRate(double rate) {
    if (rate >= 0) {
        this->consumptionRate = rate;
    }
    else {
        std::cerr << "Error. Consumption rate cannot be a negative number\n";
    }
}