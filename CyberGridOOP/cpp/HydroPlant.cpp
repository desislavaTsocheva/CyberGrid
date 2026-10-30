#include "HydroPlant.hpp"
#include <iostream>

HydroPlant::HydroPlant() : EnergyNode(), waterFlowRate(0.0) {}

HydroPlant::HydroPlant(std::string name, double capacity, double flowRate)
    : EnergyNode(name, capacity), waterFlowRate(flowRate) {}

double HydroPlant::producePower() const {
    double factor = waterFlowRate / 10.0;
    if (factor > 1.0) factor = 1.0;
    return capacity * factor;
}

void HydroPlant::update(double step) {
    (void)step;
}

double HydroPlant::getWaterFlowRate() const {
    return waterFlowRate;
}

void HydroPlant::setWaterFlowRate(double rate) {
    if (rate >= 0) {
        waterFlowRate = rate;
    } else {
        std::cerr << "Water flow rate cannot be negative.\n";
    }
}