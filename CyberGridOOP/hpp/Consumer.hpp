#ifndef CONSUMER_HPP
#define CONSUMER_HPP

#include "EnergyNode.hpp"

class Consumer : public EnergyNode {
private:
    double consumptionRate;

public:
    Consumer();
    Consumer(const std::string& name, double maxCapacity, double consumptionRate);

    double producePower() const override;
    void update(double timeStep) override;

    double getConsumptionRate() const;
    void setConsumptionRate(double rate);
};

#endif