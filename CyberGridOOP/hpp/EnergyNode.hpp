#ifndef ENERGY_NODE_HPP
#define ENERGY_NODE_HPP

#include <string>
#include <iostream>

class EnergyNode {
protected:
    std::string name;
    double capacity;

public:
    EnergyNode();
    EnergyNode(std::string name, double capacity);
    virtual ~EnergyNode() = default;

    std::string getName() const;
    void setName(const std::string& name);

    double getCapacity() const;
    void setCapacity(double capacity);

    virtual double producePower() const = 0;
    virtual void update(double step) = 0;

    friend std::ostream& operator<<(std::ostream& os, const EnergyNode& node);
    bool operator<(const EnergyNode& other) const;
};

#endif