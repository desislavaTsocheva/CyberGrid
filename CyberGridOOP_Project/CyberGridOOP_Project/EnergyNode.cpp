#include "EnergyNode.hpp"

// Implementation of EnergyNode class methods
// Default constructor initializing name to an empty string and capacity to 0.0
EnergyNode::EnergyNode() : name(""), capacity(0.0) {}

// Parameterized constructor initializing name and capacity with provided values
EnergyNode::EnergyNode(std::string name, double capacity) : name(name), capacity(capacity) {}

std::string EnergyNode::getName() const {
    return name;
}

void EnergyNode::setName(const std::string& name) {
    this->name = name;
}

double EnergyNode::getCapacity() const {
    return capacity;
}

void EnergyNode::setCapacity(double capacity) {
    if (capacity >= 0) {
        this->capacity = capacity;
    }
    else {
        std::cerr << "Capacity cannot be negative. Value not set.\n";
    }
}

// Overloaded operator<< to print the details of the energy node
// This function allows easy printing of EnergyNode objects to output streams
// It is declared as a friend function in the EnergyNode class to access private members
std::ostream& operator<<(std::ostream& os, const EnergyNode& node) {
    os << "Node [Name: " << node.name << ", Capacity: " << node.capacity << " kW]";
    return os;
}

// Overloaded operator< to compare energy nodes based on their capacity
// This operator allows sorting and comparison of EnergyNode objects
// It is useful for algorithms that require ordering of nodes, such as sorting by capacity
// The operator returns true if the current node's capacity is less than the other node's capacity
// It is declared as a member function of the EnergyNode class
// The operator does not modify the state of the objects being compared
// It is marked as const to indicate that it does not change the state of the object
bool EnergyNode::operator<(const EnergyNode& other) const {
    return this->capacity < other.capacity;
}