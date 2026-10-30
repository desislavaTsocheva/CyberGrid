#include "EnergyGrid.hpp"
#include "Consumer.hpp"
#include <iostream>
#include <algorithm>

EnergyGrid::EnergyGrid() {}

void EnergyGrid::addNode(std::unique_ptr<EnergyNode> node) {
    nodes.push_back(std::move(node));
}

void EnergyGrid::simulateStep(double timeStep) {
    double totalProduction = 0.0;
    double totalConsumption = 0.0;

    std::vector<Battery*> batteries;

    for (const auto& node : nodes) {
        node->update(timeStep);

        totalProduction += node->producePower();

        Consumer* consumer = dynamic_cast<Consumer*>(node.get());
        if (consumer != nullptr) {
            totalConsumption += consumer->getConsumptionRate() * timeStep;
        }

        Battery* battery = dynamic_cast<Battery*>(node.get());
        if (battery != nullptr) {
            batteries.push_back(battery);
        }
    }

    double balance = totalProduction - totalConsumption;

    if (balance > 0) {
        std::cout << "ENERGY SURPLUS: " << balance
            << " kWh. Charging batteries...\n";

        for (auto battery : batteries) {
            if (balance <= 0) break;
            battery->charge(balance);
            balance = 0;
        }
    }
    else if (balance < 0) {
        double deficit = -balance;

        std::cout << "ENERGY DEFICIT: " << deficit
            << " kWh. Discharging batteries...\n";

        for (auto battery : batteries) {
            if (deficit <= 0) break;
            double taken = battery->discharge(deficit);
            deficit -= taken;
        }

        if (deficit > 0) {
            std::cout << "WARNING: Batteries are depleted! Remaining deficit: "
                << deficit << " kWh.\n";
        }
    }
    else {
        std::cout << "Balanced state! Production exactly matches consumption.\n";
    }
}

void EnergyGrid::printGridStatus() const {
    std::cout << "\n=== ENERGY GRID STATUS ===\n";

    for (const auto& node : nodes) {
        std::cout << *node << "\n";
    }

    std::cout << "==========================\n";
}

void EnergyGrid::sortByCapacity() {
    std::sort(nodes.begin(), nodes.end(), [](const auto& a, const auto& b) {
        return *a < *b;
        });
}