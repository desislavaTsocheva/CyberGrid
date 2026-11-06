#ifndef NODE_FACTORY_HPP
#define NODE_FACTORY_HPP

#include <memory>
#include <string>
#include "EnergyNode.hpp"

// NodeFactory class to create energy nodes based on type
class NodeFactory {
public:
	// Static method to create an energy node based on the specified type
	// The method takes the type of the node, name, capacity, and an optional extra parameter
	// It returns a unique pointer to the created EnergyNode
    static std::unique_ptr<EnergyNode> createNode(const std::string& type, const std::string& name, double capacity, double extra = 0.0);
};

#endif