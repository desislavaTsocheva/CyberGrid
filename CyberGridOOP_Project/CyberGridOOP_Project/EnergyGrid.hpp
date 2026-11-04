#ifndef ENERGY_GRID_HPP
#define ENERGY_GRID_HPP

#include <vector>
#include <memory>
#include <algorithm>
#include <string>
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
    void removeNode(const std::string& name);
    void simulateStep(double timeStep);
    void sortByCapacity();
    const std::vector<std::unique_ptr<EnergyNode>>& getNodes() const;
};

#endif