#ifndef CANNOT_INSERT_CLIENT_EXCEPTION_HPP
#define CANNOT_INSERT_CLIENT_EXCEPTION_HPP

#include "routeException.hpp"
#include <iostream>
#include <exception>
#include <string>

class CannotInsertClientException : public RouteException{

    std::string message;

public:
    CannotInsertClientException(){}

    virtual const char * what() const noexcept override{

        return "Nie można wstawić klienta do trasy";
    }

};


#endif
