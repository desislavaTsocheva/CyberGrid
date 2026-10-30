#include <iostream>
#include <memory>
#include "SolarPanel.hpp"
#include "Battery.hpp"
#include "Consumer.hpp"
#include "EnergyGrid.hpp"

int main() {
    std::cout << "=== STARTING ENERGY GRID SIMULATION ===\n";

    EnergyGrid grid;

    grid.addNode(std::make_unique<SolarPanel>("Solar-Park-1", 50.0, 0.8));
    grid.addNode(std::make_unique<Battery>("Home-Battery", 10.0, 100.0));
    grid.addNode(std::make_unique<Consumer>("City-Factory", 50.0, 25.0));

    grid.printGridStatus();

    std::cout << "\n--- TIME SIMULATION (STEP BY STEP) ---\n";

    for (int hour = 1; hour <= 3; ++hour) {
        std::cout << "\n[ Hour " << hour << " ]";

        grid.simulateStep(1.0);
    }

    std::cout << "\n\nGrid elements sorted by capacity:\n";

    grid.sortByCapacity();
    grid.printGridStatus();

    std::cout << "\nSimulation completed successfully!\n";

    return 0;
}