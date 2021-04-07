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

    float objectiveFunction(float route_coeff=1000.f, float distance_coeff=100.f, float time_coeff=100.0f){

        return routeNumber * route_coeff + distance_coeff * routeCostDiff + time_coeff * routeDistanceDiff;
    }
};


class Route{

    private:
    
        std::vector<routeCustomer> route;
        float totalRouteCost;
        float totalTimeCost;
        float totalCapacity;
        float MAX_CAPACITY;
        Customer * magazine;

        float calcWaitingTime(const unsigned int i);
        float calcWaitingTime(const routeCustomer &prec, const Customer &succ);

        static bool checkIfPossiblePushRoute(Route &r, const unsigned int _i, float &routeOffsetIn, float &routeOffsetOut);
                /////////function checks if route can be moved in time by routeOffsetIn (positive -> forward, negative -> backward)
                /////////_i is the begin index
                /////////route offsetOut is -1 if insertion was not possible, and if was it returns 
                /////////value by which the route was moved in time due to insertion
                /////////positive value means extension of a route, negative value implies shorten of a route
        bool execInsertion();
        bool checkIfCanReturnInTime();

    public:
        
        //defines types of moves you can perfom on route
        enum Move{
            DELETE_INSERT, SWAP
        };

        static const int NMOVES = 2;

        Route(ProviderInfo & pi) : MAX_CAPACITY(pi.truck_capacity){

            magazine = new Customer(0, pi.warehouse_x, pi.warehouse_y, 0.0f, 0.0f, pi.due_date, 0.0f);
            this->route.clear();
            this->route.shrink_to_fit();
            this->route.push_back({magazine, 0.f, 0.f, 0.f});
            this->route.push_back({magazine, 0.f, 0.f, 0.f});


            this->totalRouteCost = 0;
            this->totalTimeCost = 0;
            this->totalCapacity = 0;
        }

        Route(const Route & route){

            this->magazine = new Customer(
                route.magazine->id, route.magazine->x, 
                route.magazine->y, route.magazine->q, 
                route.magazine->e, route.magazine->l, route.magazine->d
            );

            this->route = route.route;

            this->route.front().customer = this->magazine;
            this->route.back().customer = this->magazine;

            this->totalCapacity = route.totalCapacity;
            this->totalTimeCost = route.totalTimeCost;
            this->totalRouteCost = route.totalRouteCost;
            this->MAX_CAPACITY = route.MAX_CAPACITY;
        }

        virtual ~Route(){
            delete magazine;
        }

        routeCustomer & operator[](std::size_t _i);
        const routeCustomer & operator<=(const Route & r);
        const routeCustomer & operator>=(const Route & r);

        float newBeginTime(const unsigned int i);
        static float newBeginTime(const routeCustomer &prec, const Customer &succ);
        static float newBeginTime(const Customer &prec, const Customer &succ, const float beginTimePrec);

        routeCustomer & getLastCustomer();
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