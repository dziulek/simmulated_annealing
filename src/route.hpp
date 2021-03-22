#ifndef ROUTE_HPP
#define ROUTE_HPP

#include "customer.hpp"
#include "constants.hpp"
#include "Exceptions.hpp"
#include <vector>
#include <algorithm>
#include <cmath>

struct ProviderInfo{

    unsigned int vehicle_number;
    float warehouse_x;
    float warehouse_y;
    float truck_capacity;
    float due_date;
};


struct routeCustomer{

    Customer* customer;
    float beginTime;
    float distance;
    float waitingTime;
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
        float totalCapacity;
        float MAX_CAPACITY;

        float calcWaitingTime(const unsigned int i);
        float calcWaitingTime(const routeCustomer &prec, const Customer &succ);

        static bool checkIfPossiblePushRoute(Route &r, const unsigned int _i, float &routeOffsetIn, float &routeOffsetOut);
                /////////function checks if route can be moved in time by routeOffsetIn (positive -> forward, negative -> backward)
                /////////_i is the begin index
                /////////route offsetOut is -1 if insertion was not possible, and if was it returns 
                /////////value by which the route was moved in time due to insertion
                /////////positive value means extension of a route, negative value implies shorten of a route
        bool execInsertion();

    public:

        Route(ProviderInfo & pi) : MAX_CAPACITY(pi.truck_capacity){

            Customer * cust = new Customer(0, pi.warehouse_x, pi.warehouse_y, 0.0f, 0.0f, pi.due_date, 0.0f);
            this->route.push_back(routeCustomer{cust, 0.0f, 0.0f, 0.0f});

            this->totalRouteCost = 0;
            this->totalTimeCost = 0;
            this->totalCapacity = 0;
        }

        virtual ~Route(){
            delete route[0].customer;
        }

        routeCustomer & operator[](std::size_t _i);
        const routeCustomer & operator<=(const Route & r);
        const routeCustomer & operator>=(const Route & r);

        inline float newBeginTime(const unsigned int i){

            return std::max(this->route[i].customer->e,
                            this->route[i - 1].beginTime + Customer::dist(*route[i - 1].customer, *route[i].customer) + this->route[i -1].customer->d);
        }
        static inline float newBeginTime(const routeCustomer &prec, const Customer &succ){

            return std::max(succ.e, prec.beginTime + Customer::dist(succ, *prec.customer) + prec.customer->d);
        }
        
        static inline float newBeginTime(const Customer &prec, const Customer &succ, const float beginTimePrec){

            return std::max(succ.e, beginTimePrec + Customer::dist(succ, prec) + prec.d);
        }

        float getRemainingCapacity() const { return this->MAX_CAPACITY - this->totalCapacity; }
        float getRouteCost() const { return this->totalRouteCost; }
        float getTimeCost() const;
        unsigned int getSizeOfroute() const { return this->route.size(); }

        bool appendCustomer(Customer &c);
        bool insertCustomerIntoRoute(Customer &c, const unsigned int i);
        bool deleteCustomer(const Customer &c);
        bool deleteCustomer(const unsigned int i);

        static bool checkIfPossibleDeleteInsert(Route &r1, const unsigned int _i, Route &r2, const unsigned int _j, routeImprovement &score);
        static bool checkIfPossibleSwapBetweenRoutes( Route &r1, const unsigned int _i, Route &r2, const unsigned int _j, routeImprovement &score);
        static bool execDeleteInsert(Route &r1, const unsigned int _i, Route &r2, const unsigned int _j);
        static bool execSwapBetweenRoutes( Route &r1, const unsigned int _i,  Route &r2, const unsigned int _j);

};

#endif