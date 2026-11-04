#ifndef NODE_FACTORY_HPP
#define NODE_FACTORY_HPP

#include <memory>
#include <string>
#include "EnergyNode.hpp"

class NodeFactory {
public:
    static std::unique_ptr<EnergyNode> createNode(const std::string& type, const std::string& name, double capacity, double extra = 0.0);
};

#endif