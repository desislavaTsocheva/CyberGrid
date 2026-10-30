#include "EnergyNode.hpp"
#include <iostream>

EnergyNode::EnergyNode() :name(""), capacity(0.0) {}

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
		std::cerr << "Capacity cannot be negative. Value not set." << std::endl;
	}
}

std::ostream& operator<<(std::ostream& os, const EnergyNode& node) {
	os << "Node [Name: " << node.name << ", Capacity: " << node.capacity << " kW]";
	return os;
}

bool EnergyNode::operator<(const EnergyNode& other) const {
	return this->capacity < other.capacity;
}

