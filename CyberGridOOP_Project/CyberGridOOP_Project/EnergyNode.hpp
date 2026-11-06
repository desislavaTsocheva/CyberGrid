#ifndef ENERGY_NODE_HPP
#define ENERGY_NODE_HPP

// Include necessary headers
// Include string for name and capacity representation
#include <string>

// Include iostream for printing node details
#include <iostream>

//Abstract base class for all energy nodes in the grid
class EnergyNode {
protected:
    std::string name;
    double capacity;

public:
	//Constructor without parameters
    EnergyNode();
	//Constructor with parameters

    EnergyNode(std::string name, double capacity);
	//Virtual destructor to ensure proper cleanup of derived classes
    virtual ~EnergyNode() = default;

	//Pure virtual function to get the type of the energy node
	//Need to be implemented in derived classes
    virtual std::string getType() const = 0;

	//Getters and setters for name and capacity
    std::string getName() const;
    void setName(const std::string& name);

    double getCapacity() const;
    void setCapacity(double capacity);

	//Pure virtual functions to produce and update power, to be implemented in derived classes
    virtual double producePower() const = 0;
    virtual void update(double step) = 0;

	//Overloaded operator<< to print the details of the energy node
	//Overloaded operator< to compare energy nodes based on their capacity
	//Friend function to allow access to private members for printing
    friend std::ostream& operator<<(std::ostream& os, const EnergyNode& node);
    bool operator<(const EnergyNode& other) const;
};

#endif