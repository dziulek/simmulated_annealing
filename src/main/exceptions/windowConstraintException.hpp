#ifndef WINDOW_CONSTRAINT_EXCEPTION_HPP
#define WINDOW_CONSTRAINT_EXCEPTION_HPP

#include "routeException.hpp"
#include "../customer.hpp"

class WindowConstraintException : public RouteException{

private:

    float close_window;
    float truck_arrival;
    Customer * customer;
    std::string message;
    
public:

    WindowConstraintException(){}
    WindowConstraintException(float c_window, float arrival, Customer *c) : close_window(c_window), truck_arrival(arrival), customer(c){

        message = "broken window constraint in customer of id: " + std::to_string(customer->id);
        message += "\ntruck_arrival at " + std::to_string(truck_arrival) + ", when window is closed at " + std::to_string(customer->l);
    }

    virtual const char * what() const noexcept override{

        return message.c_str();
    }
};

#endif