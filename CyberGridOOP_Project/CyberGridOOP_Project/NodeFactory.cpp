#include <memory>
#include "NodeFactory.hpp"
#include "Battery.hpp"
#include "Consumer.hpp"
#include "GridExeptions.hpp"
#include "SolarPanel.hpp"

std::unique_ptr<EnergyNode> NodeFactory::createNode(const std::string& type, const std::string& name, double capacity, double extra) {
    if (type == "solar") {
        return std::make_unique<SolarPanel>(name, capacity, extra);
    }
    else if (type == "battery") {
        return std::make_unique<Battery>(name, capacity, extra);
    }
    else if (type == "consumer") {
        return std::make_unique<Consumer>(name, capacity, capacity);
    }
    else {
        throw GridException("Непознат тип възел за фабриката!");
    }
}