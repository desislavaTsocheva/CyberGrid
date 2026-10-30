#ifndef ENERGY_GRID_HPP
#define ENERGY_GRID_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include "EnergyNode.hpp"
#include "SolarPanel.hpp"
#include "Battery.hpp"
#include "Consumer.hpp"

class EnergyGrid {
private:
    std::vector<std::unique_ptr<EnergyNode>> nodes;

public:
    EnergyGrid();

    void addNode(std::unique_ptr<EnergyNode> node);
    void simulateStep(double timeStep);
    void printGridStatus() const;
    void sortByCapacity();
    void saveToJson(const std::string& filename, int hour, double production, double consumption) const;
};


#endif