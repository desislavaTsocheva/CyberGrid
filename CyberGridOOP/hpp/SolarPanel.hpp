#ifndef SOLAR_PANEL_HPP
#define SOLAR_PANEL_HPP

#include "EnergyNode.hpp"

class SolarPanel : public EnergyNode {
private:
    double sunlightIntensity;

public:
    SolarPanel();
    SolarPanel(std::string name, double capacity, double sunlightIntensity);

    double getSunlightIntensity() const;
    void setSunlightIntensity(double intensity);

    double producePower() const override;
    void update(double step) override;
};

#endif