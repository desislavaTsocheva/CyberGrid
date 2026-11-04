#define NOMINMAX
#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <vector>
#include <memory>
#include <iomanip>
#include <string>
#include <sstream>
#include <deque>
#include <map>
#include <fstream>
#include <algorithm>
#include <limits>
#include <conio.h>
#include <windows.h>
#include "EnergyGrid.hpp"
#include "NodeFactory.hpp"

#define ESCAPE 27

const int CONSOLE_WIDTH = 100;
const int CONSOLE_HEIGHT = 40;
const int INNER_WIDTH = 98;
const int HISTORY_LIMIT = 20;

namespace Color {
    const std::string RESET = "\033[0m";
    const std::string GREEN = "\033[1;32m";
    const std::string DARK = "\033[38;5;240m";
    const std::string CYAN = "\033[1;36m";
    const std::string BLUE = "\033[1;34m";
    const std::string RED = "\033[1;31m";
    const std::string YELLOW = "\033[1;33m";
    const std::string ORANGE = "\033[38;5;208m";
    const std::string WHITE = "\033[1;37m";
    const std::string PURPLE = "\033[1;35m";
}

struct SimulationData {
    int step;
    double production;
    double consumption;
    double balance;
};

std::deque<SimulationData> history;
std::deque<std::string> eventLog;

void addEvent(const std::string& event) {
    eventLog.push_front(event);
    if (eventLog.size() > 10) {
        eventLog.pop_back();
    }
}

void addHistory(int step, double production, double consumption) {
    SimulationData data;
    data.step = step;
    data.production = production;
    data.consumption = consumption;
    data.balance = production - consumption;
    history.push_back(data);
    if (history.size() > HISTORY_LIMIT) {
        history.pop_front();
    }
}

void setupConsoleWindow() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    HWND hwnd = GetConsoleWindow();

    SetConsoleTitleA("CYBER GRID // HACKER TERMINAL v5.0");

    DWORD mode = 0;
    if (GetConsoleMode(hConsole, &mode)) {
        mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, mode);
    }

    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 100;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    SMALL_RECT tempWindow = { 0, 0, 79, 24 };
    SetConsoleWindowInfo(hConsole, TRUE, &tempWindow);

    COORD bufferSize = { CONSOLE_WIDTH, CONSOLE_HEIGHT };
    SetConsoleScreenBufferSize(hConsole, bufferSize);

    SMALL_RECT windowSize = { 0, 0, CONSOLE_WIDTH - 1, CONSOLE_HEIGHT - 1 };
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);

    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    if (GetCurrentConsoleFontEx(hConsole, FALSE, &fontInfo)) {
        int fontWidth = fontInfo.dwFontSize.X;
        int fontHeight = fontInfo.dwFontSize.Y;

        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);

        int width = CONSOLE_WIDTH * fontWidth;
        int height = CONSOLE_HEIGHT * fontHeight + 40;

        int x = (screenWidth - width) / 2;
        int y = (screenHeight - height) / 2;

        if (x < 0) x = 0;
        if (y < 0) y = 0;

        SetWindowPos(hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
    }
}

void clearScreen() {
    system("cls");
}

void topBorder() {
    std::cout << Color::GREEN << "+" << std::string(INNER_WIDTH, '=') << "+" << Color::RESET << "\n";
}

void bottomBorder() {
    topBorder();
}

void printCentered(const std::string& text, const std::string& color = Color::GREEN) {
    int padding = (INNER_WIDTH - (int)text.length()) / 2;
    if (padding < 0) padding = 0;
    std::cout << color << std::string(padding, ' ') << text << Color::RESET << "\n";
}

void printRow(const std::string& text) {
    std::string output = text;
    if (output.length() > INNER_WIDTH) {
        output = output.substr(0, INNER_WIDTH);
    }
    std::cout << Color::GREEN << "| " << std::left << std::setw(INNER_WIDTH - 2) << output << "|" << Color::RESET << "\n";
}

void printSeparator() {
    std::cout << Color::DARK << "+" << std::string(INNER_WIDTH, '-') << "+" << Color::RESET << "\n";
}

void loadingAnimation(const std::string& message) {
    clearScreen();
    topBorder();
    printCentered("CYBER GRID // SYSTEM PROCESS", Color::CYAN);
    printSeparator();
    printCentered(message, Color::WHITE);
    std::cout << "\n";

    for (int i = 0; i <= 30; ++i) {
        int percentage = (i * 100) / 30;
        std::cout << "\r" << Color::GREEN << "        [";
        for (int j = 0; j < 30; ++j) {
            std::cout << (j < i ? "#" : "-");
        }
        std::cout << "] " << std::setw(3) << percentage << "% " << Color::RESET << std::flush;
        Sleep(20);
    }
    std::cout << "\n\n";
    printCentered("PROCESS COMPLETED", Color::GREEN);
    Sleep(350);
}

void drawEnergyChart(double production, double consumption) {
    const int chartWidth = 45;
    double maxValue = (std::max)(production, consumption);
    if (maxValue < 1) maxValue = 1;

    int productionBars = (int)((production / maxValue) * chartWidth);
    int consumptionBars = (int)((consumption / maxValue) * chartWidth);

    std::cout << "\n";
    std::cout << Color::GREEN << "  PRODUCTION  [";
    for (int i = 0; i < chartWidth; ++i) {
        std::cout << (i < productionBars ? "#" : "-");
    }
    std::cout << "] " << std::fixed << std::setprecision(1) << production << " kW" << Color::RESET << "\n";

    std::cout << Color::RED << "  CONSUMPTION [";
    for (int i = 0; i < chartWidth; ++i) {
        std::cout << (i < consumptionBars ? "#" : "-");
    }
    std::cout << "] " << consumption << " kW" << Color::RESET << "\n";
}

void drawHistoryGraph() {
    if (history.empty()) {
        std::cout << Color::YELLOW << "\n  No simulation data available yet.\n" << Color::RESET;
        return;
    }

    double maxValue = 1;
    for (const auto& data : history) {
        maxValue = (std::max)(maxValue, data.production);
        maxValue = (std::max)(maxValue, data.consumption);
    }

    const int graphHeight = 10;
    std::cout << "\n";
    printCentered("LIVE SIMULATION HISTORY", Color::CYAN);

    for (int level = graphHeight; level >= 1; --level) {
        double threshold = maxValue * level / graphHeight;
        std::cout << Color::DARK << std::setw(7) << (int)threshold << " | " << Color::RESET;

        for (const auto& data : history) {
            if (data.production >= threshold) {
                std::cout << Color::GREEN << "P " << Color::RESET;
            }
            else if (data.consumption >= threshold) {
                std::cout << Color::RED << "C " << Color::RESET;
            }
            else {
                std::cout << "  ";
            }
        }
        std::cout << "\n";
    }

    std::cout << Color::DARK << "         +" << std::string(history.size() * 2, '-') << Color::RESET << "\n";
    std::cout << Color::GREEN << "         P = PRODUCTION" << Color::RESET << "     " << Color::RED << "C = CONSUMPTION" << Color::RESET << "\n";
}

void showAnalytics(const EnergyGrid& grid) {
    clearScreen();
    topBorder();
    printCentered("GRID ANALYTICS // SYSTEM INTELLIGENCE", Color::CYAN);
    printSeparator();

    const auto& nodes = grid.getNodes();
    std::map<std::string, int> typeCount;
    double totalCapacity = 0;

    for (const auto& node : nodes) {
        typeCount[node->getType()]++;
        totalCapacity += node->getCapacity();
    }

    printRow("TOTAL CONNECTED NODES: " + std::to_string(nodes.size()));
    printRow("TOTAL INSTALLED CAPACITY: " + std::to_string(totalCapacity) + " kW");
    printSeparator();
    printCentered("NODE DISTRIBUTION", Color::YELLOW);

    for (const auto& item : typeCount) {
        printRow(item.first + " : " + std::to_string(item.second) + " units");
    }

    printSeparator();
    printRow("TOTAL SIMULATION STEPS: " + std::to_string(history.empty() ? 0 : history.back().step));
    printRow("RECORDED HISTORY: " + std::to_string(history.size()) + " samples");

    if (!history.empty()) {
        printRow("LATEST PRODUCTION: " + std::to_string(history.back().production) + " kW");
        printRow("LATEST CONSUMPTION: " + std::to_string(history.back().consumption) + " kW");
        printRow("LATEST BALANCE: " + std::to_string(history.back().balance) + " kW");
    }

    bottomBorder();
    std::cout << "\nPress any key to return...";
    _getch();
}

void showNodeInformation(const EnergyGrid& grid) {
    clearScreen();
    topBorder();
    printCentered("NETWORK NODE INSPECTOR", Color::CYAN);
    printSeparator();

    const auto& nodes = grid.getNodes();
    if (nodes.empty()) {
        printCentered("NO NODES DETECTED", Color::RED);
    }
    else {
        for (size_t i = 0; i < nodes.size(); ++i) {
            std::cout << "\n";
            printRow("NODE ID: " + std::to_string(i + 1));
            printRow("NAME: " + nodes[i]->getName());
            printRow("TYPE: " + nodes[i]->getType());
            printRow("CAPACITY: " + std::to_string(nodes[i]->getCapacity()) + " kW");

            Consumer* consumer = dynamic_cast<Consumer*>(nodes[i].get());
            if (consumer) {
                printRow("CONSUMPTION RATE: " + std::to_string(consumer->getConsumptionRate()) + " kW");
            }
            printSeparator();
        }
    }
    bottomBorder();
    std::cout << "\nPress any key to return...";
    _getch();
}

void showEventLog() {
    clearScreen();
    topBorder();
    printCentered("SYSTEM EVENT LOG", Color::CYAN);
    printSeparator();

    if (eventLog.empty()) {
        printCentered("NO SYSTEM EVENTS RECORDED", Color::YELLOW);
    }
    for (const auto& event : eventLog) {
        printRow("[EVENT] " + event);
    }
    bottomBorder();
    std::cout << "\nPress any key to return...";
    _getch();
}

void exportReport(const EnergyGrid& grid) {
    std::ofstream file("grid_report.csv");
    if (!file.is_open()) {
        addEvent("REPORT EXPORT FAILED");
        std::cout << Color::RED << "\n[ERROR] Cannot create report file." << Color::RESET;
        Sleep(1200);
        return;
    }

    file << "STEP,PRODUCTION,CONSUMPTION,BALANCE\n";
    for (const auto& data : history) {
        file << data.step << "," << data.production << "," << data.consumption << "," << data.balance << "\n";
    }

    file << "\nNODE_NAME,NODE_TYPE,CAPACITY\n";
    for (const auto& node : grid.getNodes()) {
        file << node->getName() << "," << node->getType() << "," << node->getCapacity() << "\n";
    }
    file.close();

    addEvent("GRID REPORT EXPORTED");
    std::cout << Color::GREEN << "\n[OK] Report saved as grid_report.csv" << Color::RESET;
    Sleep(1200);
}

void drawConsoleUI(const EnergyGrid& grid, double production, double consumption, int steps) {
    clearScreen();

    double balance = production - consumption;
    double load = 0;
    if (production > 0) {
        load = (consumption / production) * 100;
    }

    const auto& nodes = grid.getNodes();
    topBorder();
    printCentered(" ______  ____  ____  ______   ________  _______      ______  _______     _____  ______    ", Color::GREEN);
    printCentered(" .' ___  ||_   _||_   _||_   _ \\ |_   __  ||_   __ \\    .' ___  ||_   __ \\    |_   _||_   _ `.  ", Color::GREEN);
    printCentered("/ .'   \\_|  \\ \\  / /    | |_) |  | |_ \\_|  | |__) | / .'   \\_|  | |__) |     | |    | | `. \\ ", Color::GREEN);
    printCentered("| |         \\ \\/ /     |  __'.  |  _| _   |  __ /  | |   ____  |  __ /      | |    | |  | | ", Color::GREEN);
    printCentered("\\ `.___.'\\   _|  |_    _| |__) |_| |__/ | _| |  \\ \\_\\ `.___]  |_| |  \\ \\_  _| |_  _| |_.' /  ", Color::GREEN);
    printCentered(" `.____ .'  |______|  |_______/|________||____| |___|`._____.'|____| |___||_____||______.'  ", Color::GREEN);

    printSeparator();

    std::ostringstream status;
    status << "SYSTEM: ONLINE" << "     STEP: " << std::setw(4) << steps << "     NODES: " << std::setw(3) << nodes.size() << "     SECURITY: ACTIVE";
    printCentered(status.str(), Color::GREEN);

    printSeparator();
    printCentered("ENERGY DASHBOARD", Color::CYAN);

    std::ostringstream stats;
    stats << std::fixed << std::setprecision(1) << "PRODUCTION: " << production << " kW" << "    CONSUMPTION: " << consumption << " kW";
    printCentered(stats.str(), Color::WHITE);

    std::ostringstream balanceText;
    balanceText << std::fixed << std::setprecision(1) << "ENERGY BALANCE: " << (balance >= 0 ? "+" : "") << balance << " kW";
    printCentered(balanceText.str(), balance >= 0 ? Color::GREEN : Color::RED);

    std::ostringstream loadText;
    loadText << std::fixed << std::setprecision(1) << "GRID LOAD: " << load << "%";
    printCentered(loadText.str(), Color::YELLOW);

    drawEnergyChart(production, consumption);
    printSeparator();
    printCentered("NETWORK STATUS", Color::CYAN);

    if (nodes.empty()) {
        printCentered("WARNING: NO ACTIVE NODES", Color::RED);
    }
    else if (balance < 0) {
        printCentered("WARNING: ENERGY DEFICIT DETECTED", Color::RED);
    }
    else {
        printCentered("ALL SYSTEMS OPERATIONAL", Color::GREEN);
    }

    printSeparator();
    printCentered("CONTROL CENTER", Color::CYAN);

    printRow("[1] RUN SIMULATION         [2] ADD ENERGY NODE");
    printRow("[3] REMOVE NODE            [4] NODE INSPECTOR");
    printRow("[5] GRID ANALYTICS         [6] HISTORY GRAPH");
    printRow("[7] SYSTEM EVENT LOG       [8] EXPORT CSV REPORT");

    printSeparator();

    std::string lastEvent = eventLog.empty() ? "SYSTEM INITIALIZED" : eventLog.front();
    printRow("LAST EVENT: " + lastEvent);

    bottomBorder();

    std::cout << Color::GREEN << "\n  COMMAND INPUT > " << Color::WHITE << std::flush;
}

double readDouble(const std::string& message) {
    double value;
    while (true) {
        std::cout << Color::CYAN << message << Color::RESET;
        if (std::cin >> value && value >= 0) {
            return value;
        }
        std::cout << Color::RED << "[ERROR] Enter a valid non-negative number.\n" << Color::RESET;
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
    }
}

void addEnergyNode(EnergyGrid& grid) {
    clearScreen();
    topBorder();
    printCentered("INSTANTIATE NEW ENERGY NODE", Color::CYAN);
    printSeparator();

    std::string type;
    std::string name;

    std::cout << "\n" << Color::GREEN << "  SUPPORTED TYPES:\n" << "  [SOLAR] [BATTERY] [CONSUMER]\n\n" << Color::RESET;
    std::cout << "  Enter node type: ";
    std::cin >> type;
    std::cout << "  Enter unique name: ";
    std::cin >> name;

    double capacity = readDouble("  Enter baseline capacity (kW): ");
    double extra = readDouble("  Enter secondary parameter: ");

    try {
        loadingAnimation("ALLOCATING NODE RESOURCES...");
        grid.addNode(NodeFactory::createNode(type, name, capacity, extra));
        addEvent("NEW NODE ADDED: " + name);
        std::cout << Color::GREEN << "\n  [SUCCESS] NODE CONNECTED TO GRID." << Color::RESET;
    }
    catch (const std::exception& e) {
        addEvent("NODE CREATION FAILED: " + name);
        std::cout << Color::RED << "\n  [ERROR] " << e.what() << Color::RESET;
    }
    catch (...) {
        addEvent("NODE CREATION FAILED: " + name);
        std::cout << Color::RED << "\n  [ERROR] NODE INSTANTIATION FAILED." << Color::RESET;
    }

    std::cout << "\n\nPress any key to continue...";
    _getch();
}

void removeEnergyNode(EnergyGrid& grid) {
    clearScreen();
    topBorder();
    printCentered("TERMINATE ENERGY NODE", Color::RED);
    printSeparator();

    std::string name;
    std::cout << "\n  Enter node name to remove: ";
    std::cin >> name;

    try {
        grid.removeNode(name);
        addEvent("NODE REMOVAL REQUEST: " + name);
        std::cout << Color::YELLOW << "\n  [INFO] Removal request processed." << Color::RESET;
    }
    catch (...) {
        addEvent("NODE REMOVAL FAILED: " + name);
        std::cout << Color::RED << "\n  [ERROR] Unable to remove node." << Color::RESET;
    }

    std::cout << "\n\nPress any key to continue...";
    _getch();
}

void runSimulation(EnergyGrid& grid, double& production, double& consumption, int& steps) {
    loadingAnimation("EXECUTING GRID SIMULATION...");
    grid.simulateStep(1.0);

    production = 0;
    consumption = 0;

    for (const auto& node : grid.getNodes()) {
        production += node->producePower();
        Consumer* consumer = dynamic_cast<Consumer*>(node.get());
        if (consumer) {
            consumption += consumer->getConsumptionRate();
        }
    }

    steps++;
    addHistory(steps, production, consumption);

    std::ostringstream event;
    event << std::fixed << std::setprecision(1) << "SIMULATION #" << steps << " | PROD: " << production << " kW | CONS: " << consumption << " kW";
    addEvent(event.str());
}

int main() {
    setupConsoleWindow();

    EnergyGrid grid;
    double currentProduction = 0;
    double currentConsumption = 0;
    int steps = 0;

    addEvent("CYBER GRID INITIALIZED");
    addEvent("SECURITY PROTOCOL ACTIVATED");
    addEvent("SYSTEM READY");

    while (true) {
        drawConsoleUI(grid, currentProduction, currentConsumption, steps);

        int key = _getch();
        if (key == ESCAPE) {
            break;
        }

        switch (key) {
        case '1':
            runSimulation(grid, currentProduction, currentConsumption, steps);
            break;
        case '2':
            addEnergyNode(grid);
            break;
        case '3':
            removeEnergyNode(grid);
            break;
        case '4':
            showNodeInformation(grid);
            break;
        case '5':
            showAnalytics(grid);
            break;
        case '6':
            clearScreen();
            topBorder();
            drawHistoryGraph();
            bottomBorder();
            std::cout << "\nPress any key to return...";
            _getch();
            break;
        case '7':
            showEventLog();
            break;
        case '8':
            exportReport(grid);
            break;
        default:
            break;
        }
    }

    clearScreen();
    topBorder();
    printCentered("CYBER GRID SHUTDOWN", Color::RED);
    printSeparator();
    printCentered("TERMINATING SYSTEM PROCESSES...", Color::YELLOW);
    Sleep(500);
    printCentered("ALL OPERATIONS COMPLETED.", Color::GREEN);
    printCentered("THANK YOU FOR USING CYBER GRID v5.0", Color::CYAN);
    bottomBorder();
    Sleep(1500);

    return 0;
}