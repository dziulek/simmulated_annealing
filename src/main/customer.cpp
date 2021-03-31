#include "customer.hpp"

std::ostream & operator<<(std::ostream & os, const Customer & customer){

    os<<"customer id: " << customer.id << std::endl;
    os<<"x coordianate: " << customer.x << std::endl;
    os<<"y coordinate: " << customer.y << std::endl;
    os<<"demand: " << customer.q << std::endl;
    os<<"open window: " << customer.e << std::endl;
    os<<"close window: " << customer.l << std::endl;
    os<<"service time: " << customer.d << std::endl;

    return os;
}
