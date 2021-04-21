#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "customer.hpp"
#include "route.hpp"
#include <vector>
#include <memory>
#include <math.h>

class CRPTW_Solution{

private:

    std::vector<Route *> routes;
    unsigned int nOfRoutes;
    float totalDistance;
    float totalTime;
    ProviderInfo * providerInfo;
    
    

public:

    CRPTW_Solution(){

        nOfRoutes = 0;
        totalDistance = 0.0f;
        totalTime = 0.0f;
    }
    CRPTW_Solution(ProviderInfo & provInfo){

        this->providerInfo = &provInfo;
        nOfRoutes = 0;
        totalDistance = 0.0f;
        totalTime = 0.0f;
    }
    virtual ~CRPTW_Solution(){
        
        for(auto & route : this->routes)
            delete route;
    }

    bool operator<(CRPTW_Solution & solution);
    bool operator>(CRPTW_Solution & solution);

    CRPTW_Solution(const CRPTW_Solution & solution){

        this->providerInfo = solution.providerInfo;

        this->routes.clear();
        this->routes.shrink_to_fit();
        for(auto & route : solution.routes){
            
            Route * newRoute = new Route(*route);
            this->routes.push_back(newRoute);
        }
        //other field
        this->nOfRoutes = solution.nOfRoutes;
        this->totalDistance = solution.totalDistance;
        this->totalTime = solution.totalTime;
    }

    Route & getRoute(unsigned int i);
    Route & addRoute();
    Route & addRoute(const Route & route);
    void deleteRoute(unsigned int i);

    float getTotalDistance() const;
    float getTotalTime() const;
    unsigned int getNOfRoutes() const;

    void clearSolution();
    void addProviderInfo(ProviderInfo & provInfo);

    float objectiveFunction(float route_coeff=beta1, float distance_coeff=beta2, float time_coeff=beta3);

    friend class TabuList;

    static bool isValid(const CRPTW_Solution & solution);

    friend std::ostream & operator<<(std::ostream & os, const CRPTW_Solution & solution);
};

#endif