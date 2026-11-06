#ifndef DIESEL_GENERATOR_HPP
#define DIESEL_GENERATOR_HPP

#include "EnergyNode.hpp"

// DieselGenerator class derived from EnergyNode
class DieselGenerator : public EnergyNode {
private:
    double fuelLevel;
    double fuelConsumptionRate;

public:
    DieselGenerator();
    DieselGenerator(std::string name, double capacity, double fuelLevel, double consumptionRate);

    double getFuelLevel() const;
    void refuel(double amount);

    double producePower() const override;
    void update(double step) override;
    std::string getType() const override { return "DieselGenrator"; }
};

#endif