#include <algorithm>
#include "EnergyGrid.hpp"

EnergyGrid::EnergyGrid() = default;

void EnergyGrid::addNode(std::unique_ptr<EnergyNode> node) {
    nodes.push_back(std::move(node));
}

void EnergyGrid::removeNode(const std::string& name) {
    nodes.erase(std::remove_if(nodes.begin(), nodes.end(),
        [&name](const std::unique_ptr<EnergyNode>& node) {
            return node->getName() == name;
        }), nodes.end());
}

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

void EnergyGrid::sortByCapacity() {
    std::sort(nodes.begin(), nodes.end(), [](const std::unique_ptr<EnergyNode>& a, const std::unique_ptr<EnergyNode>& b) {
        return *a < *b;
        });
}

const std::vector<std::unique_ptr<EnergyNode>>& EnergyGrid::getNodes() const {
    return nodes;
}