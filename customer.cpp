#include "customer.hpp"
#include <cmath>
#include <iostream>


float Customer::dist(const Customer &c1, const Customer &c2){

    return sqrt( (c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y));
}