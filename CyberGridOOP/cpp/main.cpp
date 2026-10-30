#include <iostream>
#include <memory>
#include "SolarPanel.hpp"
#include "WindTurbine.hpp"
#include "DieselGenerator.hpp"
#include "HydroPlant.hpp"
#include "Battery.hpp"
#include "Consumer.hpp"
#include "EnergyGrid.hpp"

int main() {
    std::cout << "=== STARTING SMART ENERGY GRID SIMULATION ===\n";

    EnergyGrid grid;

    grid.addNode(std::make_unique<SolarPanel>("Solar-Park-1", 50.0, 0.8));
    grid.addNode(std::make_unique<WindTurbine>("Wind-Turbine-1", 40.0, 12.0));
    grid.addNode(std::make_unique<DieselGenerator>("Backup-Generator", 30.0, 50.0, 5.0));
    grid.addNode(std::make_unique<HydroPlant>("Micro-Hydro-1", 60.0, 8.0));
    grid.addNode(std::make_unique<Battery>("Home-Battery", 150.0, 0.95));
    grid.addNode(std::make_unique<Consumer>("City-Factory", 80.0, 40.0));

    std::cout << "\nInitial Grid Status:\n";
    grid.printGridStatus();

    std::cout << "\n--- TIME SIMULATION (STEP BY STEP) ---\n";

    for (int hour = 1; hour <= 3; ++hour) {
        std::cout << "\n[ Hour " << hour << " ]";
        grid.simulateStep(1.0);
        grid.saveToJson("grid_state.json", hour, 150.0, 40.0);
    }

    std::cout << "\nGrid elements sorted by capacity:\n";
    grid.sortByCapacity();
    grid.printGridStatus();

    std::cout << "\nSimulation completed successfully!\n";

    return 0;
}