#ifndef ENERGY_NODE_HPP
#define ENERGY_NODE_HPP

#include <string>

class EnergyNode {
protected:
	std::string name;
	double capacity;

public:
	std::string getName() const;
	void setName(const std::string& name);

	double getCapacity() const;
	void setCapacity(double capacity);

	EnergyNode();
	EnergyNode(std::string name, double capacity);

	virtual ~EnergyNode()= default; 

	virtual double producePower() const = 0;
	virtual void update(double step) = 0;

	friend std::ostream& operator<<(std::ostream& os, const EnergyNode& node);

	bool operator<(const EnergyNode& other) const;
};
#endif