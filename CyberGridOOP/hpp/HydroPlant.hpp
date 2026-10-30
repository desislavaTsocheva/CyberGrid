#ifndef HYDRO_PLANT_HPP
#define HYDRO_PLANT_HPP

#include "EnergyNode.hpp"

class HydroPlant : public EnergyNode {
private:
    double waterFlowRate;

public:
    HydroPlant();
    HydroPlant(std::string name, double capacity, double flowRate);

    double getWaterFlowRate() const;
    void setWaterFlowRate(double rate);

    double producePower() const override;
    void update(double step) override;
};

#endif