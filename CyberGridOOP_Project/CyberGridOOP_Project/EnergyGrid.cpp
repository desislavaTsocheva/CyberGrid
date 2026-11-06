#include <algorithm>
#include "EnergyGrid.hpp"

// Constructor
EnergyGrid::EnergyGrid() = default;

// Adds a new energy node to the grid
// The node is passed as a unique_ptr to ensure that the grid takes ownership of the node
// This prevents memory leaks and ensures proper resource management
// The vector allows for dynamic resizing and efficient access to the nodes
// The function uses std::move to transfer ownership of the unique_ptr into the vector
// This ensures that the node is properly managed and will be automatically deleted when the grid is destroyed
// The function does not return any value, as the node is added to the grid and managed internally
// The function does not check for duplicate names, as it is assumed that the caller will ensure unique names for each node
// The function does not throw any exceptions, as it is assumed that the caller will handle any errors related to node creation or management
void EnergyGrid::addNode(std::unique_ptr<EnergyNode> node) {
    nodes.push_back(std::move(node));
}

// Removes an energy node from the grid by name
// The function searches for the node with the specified name and removes it from the vector
// The function uses std::remove_if to find the node with the matching name and erase it from the vector
// The function does not return any value, as the node is removed from the grid and managed internally
// The function does not check for the existence of the node, as it is assumed that the caller will ensure that the node exists before attempting to remove it
// The function does not free the memory of the removed node, as it is managed by the unique_ptr and will be automatically deleted when it goes out of scope
void EnergyGrid::removeNode(const std::string& name) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
        [&name](const std::unique_ptr<EnergyNode>& node) {
            return node->getName() == name;
        }), nodes.end());
}

// Simulates a time step in the energy grid
// The function iterates through all nodes and calls their update method to simulate their behavior over the specified time step
// The function calculates the total production and consumption of energy in the grid
// The function then calculates the net energy balance and distributes excess energy to batteries or draws from batteries to cover deficits
void EnergyGrid::simulateStep(double timeStep) {
    double totalProduction = 0.0;
    double totalConsumption = 0.0;

    for (const auto& node : nodes) {
        node->update(timeStep);
        double p = node->producePower();
        if (p > 0) totalProduction += p;
        else totalConsumption += (-p);
    }

    double netEnergy = totalProduction - totalConsumption;
    if (netEnergy > 0) {
        for (const auto& node : nodes) {
            Battery* b = dynamic_cast<Battery*>(node.get());
            if (b && netEnergy > 0) {
                double spaceBefore = b->getMaxCapacity() - b->getCurrentCharge();
                b->charge(netEnergy);
                double spaceAfter = b->getMaxCapacity() - b->getCurrentCharge();
                netEnergy -= (spaceBefore - spaceAfter);
            }
        }
    }
    else if (netEnergy < 0) {
        double deficit = -netEnergy;
        for (const auto& node : nodes) {
            Battery* b = dynamic_cast<Battery*>(node.get());
            if (b && deficit > 0) {
                double taken = b->discharge(deficit);
                deficit -= taken;
            }
        }
    }
}

// Sorts the energy nodes in the grid by their capacity
// The function uses the overloaded operator< defined in the EnergyNode class to compare nodes based on their capacity
// The function uses std::sort to sort the vector of unique_ptrs to EnergyNode objects
// The function does not return any value, as the nodes are sorted in place within the vector
void EnergyGrid::sortByCapacity() {
    std::sort(nodes.begin(), nodes.end(), [](const std::unique_ptr<EnergyNode>& a, const std::unique_ptr<EnergyNode>& b) {
        return *a < *b;
        });
}

// Returns a const reference to the vector of energy nodes
// This allows external code to access the nodes without modifying the grid's internal state
// The function returns a const reference to the vector of unique_ptrs to EnergyNode objects
// The function does not allow modification of the vector or the nodes, ensuring encapsulation and data integrity
const std::vector<std::unique_ptr<EnergyNode>>& EnergyGrid::getNodes() const {
    return nodes;
}