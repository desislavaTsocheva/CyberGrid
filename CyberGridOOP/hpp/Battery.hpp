#ifndef BATTERY_HPP
#define BATTERY_HPP

#include "EnergyNode.hpp"

class Battery : public EnergyNode {
private:
    double currentCharge;
    double maxCapacity;
    double efficiency;

public:
    Battery();
    Battery(std::string name, double maxCapacity, double efficiency);

    double getCurrentCharge() const;
    double getMaxCapacity() const;
    double getEfficiency() const;

    double producePower() const override;
    void update(double step) override;

    void charge(double amount);
    double discharge(double amount);
};

#endif