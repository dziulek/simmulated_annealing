#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "customer.hpp"
#include "route.hpp"
#include <vector>
#include <memory>

struct ProviderInfo{

    float warehouse_x;
    float warehouse_y;
    float truck_capacity;
};

class CRPTW_Solution{

private:

    std::vector<std::unique_ptr<Route>> routes;
    unsigned int nOfRoutes;
    float totalDistance;
    float totalTime;
    ProviderInfo * providerInfo;

public:

    CRPTW_Solution(ProviderInfo & provInfo){

        this->providerInfo = &provInfo;
        nOfRoutes = 0;
        totalDistance = 0.0f;
        totalTime = 0.0f;
    }
    virtual ~CRPTW_Solution(){}

    Route & getRoute(unsigned int i);
    Route & addRoute();
    Route & addRoute(const Route & route);
    void deleteRoute(unsigned int i);

    float getTotalDistance();
    float getTotalTime();
    unsigned int getNOfRoutes();

};

#endif