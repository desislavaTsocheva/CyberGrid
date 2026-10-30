#include "SolarPanel.hpp"
#include <iostream>

SolarPanel::SolarPanel() : EnergyNode(), sunlightIntensity(0.0) {}

SolarPanel::SolarPanel(std::string name, double capacity, double sunlightIntensity)
	: EnergyNode(name, capacity), sunlightIntensity(sunlightIntensity) {}

double SolarPanel::producePower() const {
	return capacity * sunlightIntensity;
}

void SolarPanel::update(double step) {
	sunlightIntensity += 0.01 * step; 
	if (sunlightIntensity > 1.0) {
		sunlightIntensity = 1.0; 
	}
}

double SolarPanel::getSunlightIntensity() const {
	return sunlightIntensity;
}

void SolarPanel::setSunlightIntensity(double sunlightIntensity) {
	if (sunlightIntensity >= 0.0 && sunlightIntensity <= 1.0) {
		this->sunlightIntensity = sunlightIntensity;
	}
	else {
		std::cerr << "Sunlight intensity must be between 0.0 and 1.0. Value not set." << std::endl;
	}
}



