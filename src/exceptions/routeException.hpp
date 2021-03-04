#ifndef ROUTE_EXCEPTION_HPP
#define ROUTE_EXCEPTION_HPP

#include <iostream>
#include <exception>
#include <string>

class RouteException : public std::exception{

public:
    RouteException(){}
    virtual const char * what() const noexcept override{
        return "Błędna operacja na trasie";
    }
};

#endif