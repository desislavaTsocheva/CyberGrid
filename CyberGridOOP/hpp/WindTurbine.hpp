#ifndef WIND_TURBINE_HPP
#define WIND_TURBINE_HPP

#include "EnergyNode.hpp"

class WindTurbine : public EnergyNode {
private:
    double windSpeed;

public:
    WindTurbine();
    WindTurbine(std::string name, double capacity, double windSpeed);

    double getWindSpeed() const;
    void setWindSpeed(double speed);

    double producePower() const override;
    void update(double step) override;
};

#endif