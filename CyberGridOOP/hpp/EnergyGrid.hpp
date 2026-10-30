#ifndef ENERGY_GRID_HPP
#define ENERGY_GRID_HPP

#include <vector>
#include <memory>
#include "EnergyNode.hpp"
#include "Battery.hpp" 

class EnergyGrid {
private:
    std::vector<std::unique_ptr<EnergyNode>> nodes;

public:
    EnergyGrid();

    void addNode(std::unique_ptr<EnergyNode> node);

    void simulateStep(double timeStep);

    void printGridStatus() const;

    void sortByCapacity();
};

#endif 
