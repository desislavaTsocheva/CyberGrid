#ifndef GRID_EXCEPTIONS_HPP
#define GRID_EXCEPTIONS_HPP

// Include necessary headers
// Include exception for standard exception handling
#include <exception>
#include <string>

// Custom exception class for grid-related errors
// Inherits from std::exception to allow for standard exception handling
// Stores an error message and overrides the what() method to return the message
class GridException : public std::exception {
private:
    std::string message;

public:
	// Constructor to initialize the exception with a message
	// The message is stored in a private member variable and can be retrieved using the what() method
	// The constructor takes a string message as a parameter and initializes the message member variable with it
	// The what() method is overridden to return the error message as a C-style string
	// The noexcept specifier indicates that this function does not throw exceptions
	// This allows for better performance and ensures that the exception handling mechanism does not throw additional exceptions
    GridException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif