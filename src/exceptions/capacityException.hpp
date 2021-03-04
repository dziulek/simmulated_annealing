#ifndef CAPACITY_EXCEPTION_HPP
#define CAPACITY_EXCEPTION_HPP

#include "routeException.hpp"
#include "customer.hpp"

class CapacityException : public RouteException{

private:

    float MAX_CAPACITY;
    Customer * customer;
    float routeCapacity;
    std::string message;

public:

    CapacityException(){}
    CapacityException(float max_c, Customer * c, float routecap) : MAX_CAPACITY(max_c), customer(c), routeCapacity(routecap){

        message = "Route capacity exceeded, when inserting customer of id: " + std::to_string(customer->id);
        message += "\nMAX_CAPACITY: " + std::to_string(MAX_CAPACITY);
        message += "\ncurrent route capacity: " + std::to_string(routeCapacity);
    }

    virtual const char * what() const noexcept override{
        
        return message.c_str();
    }

};

#endif