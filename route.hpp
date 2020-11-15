#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "customer.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

struct routeCustomer{

    Customer* customer;
    float beginTime;
    float distance;
    float waitingTime;
};


class Route{

    private:

        std::vector<routeCustomer> route;

        inline float newBeginTime(const unsigned int i);
        inline float newBeginTime(const routeCustomer &prec, const Customer &succ);
        float calcWaitingTime(const unsigned int i);
        float calcWaitingTime(const routeCustomer &prec, const Customer &succ);

    public:

        Route();
        bool appendCustomer(Customer &c);
        bool insertCustomerIntoRoute(Customer &c, const unsigned int i);
        bool deleteCustomer(const Customer &c);
        bool deleteCustomer(const unsigned int i);

        static bool checkIfPossibleDeleteInsert(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j);
        static bool checkIfPossibleSwapBetweenRoutes(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j);
        static bool execDeleteInsert(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j);
        static bool execSwapBetweenRoutes(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j);

};

#endif