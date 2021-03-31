#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include "customer.hpp"
#include "constants.hpp"
#include <cmath>
#include <iostream>


class Customer{

    public:

        int id;
        float x; //x coordinate
        float y; //y coordinate
        float q; //demand
        float e; //time of window opening
        float l; //time of window closing
        float d; //service time

        Customer(int id, float x, float y, float q, float e, float l, float d) : id(id), x(x), y(y), q(q), e(e), l(l), d(d){};
        static inline float dist(const Customer &c1,const Customer &c2){
            return sqrt( (c1.x - c2.x)*(c1.x - c2.x) + (c1.y - c2.y)*(c1.y - c2.y) + eps);
        }

        friend std::ostream & operator<<(std::ostream & os, const Customer & customer);

};

#endif