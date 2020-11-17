#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "customer.hpp"
#include "constants.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

struct routeCustomer{

    Customer* customer;
    float beginTime;
    float distance;
    float waitingTime;
    float remainingCapacity;
};

struct routeImprovement{

    int routeNumber;//if -1 then we have one route less, if 0 nothing changes
    float routeCostDiff;//if less than 0 then improvement is expected
    float routeDistanceDiff;//if less than 0 then imporvement is expected
};


class Route{

    private:

        std::vector<routeCustomer> route;
        float totalRouteCost;
        float totalTimeCost;

        inline float newBeginTime(const unsigned int i);
        static inline float newBeginTime(const routeCustomer &prec, const Customer &succ);
        static inline float newBeginTime(const Customer &prec, const Customer &succ, const float beginTimePrec);
        float calcWaitingTime(const unsigned int i);
        float calcWaitingTime(const routeCustomer &prec, const Customer &succ);

        static bool checkIfPossibleDeletionInsertion(const Route &r, const unsigned int _i, float &routeOffsetIn, float &routeOffsetOut);
                /////////route offsetOut is -1 if insertion was not possible, and if was it returns 
                /////////value by which the route was moved in time due to insertion
                /////////positive value means extension of a route, negative value implies shorten of a route
        bool execInsertion();

    public:

        Route();

        const routeCustomer & operator[](std::size_t _i) const;
        const routeCustomer & operator<=(const Route & r);
        const routeCustomer & operator>=(const Route & r);

        float getRemainingCapacity() const { return this->route.back().remainingCapacity; }
        float getRouteCost() const { return this->totalRouteCost; }
        float getTimeCost() const { return this->totalTimeCost; }
        unsigned int getSizeOfroute() const { return this->route.size(); }

        bool appendCustomer(Customer &c);
        bool insertCustomerIntoRoute(Customer &c, const unsigned int i);
        bool deleteCustomer(const Customer &c);
        bool deleteCustomer(const unsigned int i);

        static bool checkIfPossibleDeleteInsert(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j, routeImprovement &score);
        static bool checkIfPossibleSwapBetweenRoutes(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j, routeImprovement &score);
        static bool execDeleteInsert(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j);
        static bool execSwapBetweenRoutes(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j);

};

#endif