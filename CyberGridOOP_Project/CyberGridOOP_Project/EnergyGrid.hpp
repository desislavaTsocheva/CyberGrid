#ifndef ENERGY_GRID_HPP
#define ENERGY_GRID_HPP

// Include necessary headers
// Include vector for storing energy nodes
#include <vector>

// Include memory for smart pointers
#include <memory>

// Include algorithm for sorting nodes
#include <algorithm>
#include <string>
#include "EnergyNode.hpp"
#include "SolarPanel.hpp"
#include "Battery.hpp"
#include "Consumer.hpp"

class EnergyGrid {
private:
	// Vector to hold unique pointers to energy nodes
	// Using unique_ptr ensures that each node is owned by the grid and will be automatically deleted when the grid is destroyed
	// This prevents memory leaks and ensures proper resource management
	// The vector allows for dynamic resizing and efficient access to the nodes
    std::vector<std::unique_ptr<EnergyNode>> nodes;

public:
	// Constructor to initialize the energy grid
    EnergyGrid();

	//Adds a new energy node to the grid
	// The node is passed as a unique_ptr to ensure that the grid takes ownership of the node
    void addNode(std::unique_ptr<EnergyNode> node);

	// Removes an energy node from the grid by name
	// The function searches for the node with the specified name and removes it from the vector
    void removeNode(const std::string& name);

	// Simulates a time step in the energy grid
	// The function iterates through all nodes and calls their update method to simulate their behavior over the specified time step
    void simulateStep(double timeStep);

	// Sorts the energy nodes in the grid by their capacity
	// The function uses the overloaded operator< defined in the EnergyNode class to compare nodes based on their capacity
    void sortByCapacity();

	// Returns a const reference to the vector of energy nodes
	// This allows external code to access the nodes without modifying the grid's internal state
    const std::vector<std::unique_ptr<EnergyNode>>& getNodes() const;
};

#endif