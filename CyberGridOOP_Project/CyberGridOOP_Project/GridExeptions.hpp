#ifndef GRID_EXCEPTIONS_HPP
#define GRID_EXCEPTIONS_HPP

#include <exception>
#include <string>

class GridException : public std::exception {
private:
    std::string message;

public:
    GridException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif