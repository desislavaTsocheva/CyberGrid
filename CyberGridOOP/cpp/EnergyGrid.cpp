#include "EnergyGrid.hpp"
#include <iostream>
#include <fstream>

EnergyGrid::EnergyGrid() = default;

void EnergyGrid::addNode(std::unique_ptr<EnergyNode> node) {
    nodes.push_back(std::move(node));
}

void EnergyGrid::simulateStep(double timeStep) {
    double totalProduction = 0.0;
    double totalConsumption = 0.0;

    for (const auto& node : nodes) {
        node->update(timeStep);
        totalProduction += node->producePower();
        
        Consumer* c = dynamic_cast<Consumer*>(node.get());
        if (c) {
            totalConsumption += c->getConsumptionRate() * timeStep;
        }
    }

    std::cout << "\nTotal Production: " << totalProduction << " kW"
              << " | Total Consumption: " << totalConsumption << " kW\n";

    double netEnergy = totalProduction - totalConsumption;

    if (netEnergy > 0) {
        std::cout << "Surplus energy: " << netEnergy << " kW. Charging batteries...\n";
        for (const auto& node : nodes) {
            Battery* b = dynamic_cast<Battery*>(node.get());
            if (b && netEnergy > 0) {
                double spaceBefore = b->getMaxCapacity() - b->getCurrentCharge();
                b->charge(netEnergy);
                double spaceAfter = b->getMaxCapacity() - b->getCurrentCharge();
                netEnergy -= (spaceBefore - spaceAfter);
            }
        }
    } else if (netEnergy < 0) {
        double deficit = -netEnergy;
        std::cout << "Deficit energy: " << deficit << " kW. Discharging batteries...\n";
        for (const auto& node : nodes) {
            Battery* b = dynamic_cast<Battery*>(node.get());
            if (b && deficit > 0) {
                double taken = b->discharge(deficit);
                deficit -= taken;
            }
        }
    } else {
        std::cout << "Energy balanced perfectly.\n";
    }
}

void EnergyGrid::printGridStatus() const {
    for (const auto& node : nodes) {
        std::cout << *node << "\n";
    }
}

void EnergyGrid::sortByCapacity() {
    std::sort(nodes.begin(), nodes.end(), [](const std::unique_ptr<EnergyNode>& a, const std::unique_ptr<EnergyNode>& b) {
        return *a < *b;
    });
}

void EnergyGrid::saveToJson(const std::string& filename, int hour, double production, double consumption) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "{\n";
    file << "  \"hour\": " << hour << ",\n";
    file << "  \"totalProduction\": " << production << ",\n";
    file << "  \"totalConsumption\": " << consumption << ",\n";
    file << "  \"nodes\": [\n";

    for (size_t i = 0; i < nodes.size(); ++i) {
        file << "    {\n";
        file << "      \"name\": \"" << nodes[i]->getName() << "\",\n";
        file << "      \"capacity\": " << nodes[i]->getCapacity();
        
        // Тук оправяме nodes[i].get() вместо nodes.get()
        const Battery* b = dynamic_cast<const Battery*>(nodes[i].get());
        if (b) {
            file << ",\n      \"currentCharge\": " << b->getCurrentCharge() << ",\n";
            file << "      \"maxCapacity\": " << b->getMaxCapacity() << "\n";
        } else {
            file << "\n";
        }
        
        file << "    }" << (i < nodes.size() - 1 ? "," : "") << "\n";
    }

    file << "  ]\n";
    file << "}\n";
    file.close();
}