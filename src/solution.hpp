#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "customer.hpp"
#include "route.hpp"
#include <vector>
#include <memory>

class CRPTW_Solution{

private:

    std::vector<std::unique_ptr<Route>> routes;
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
        
    }

    bool operator<(CRPTW_Solution & solution);
    bool operator>(CRPTW_Solution & solution);

    CRPTW_Solution(const CRPTW_Solution & solution){
        
        if(this->providerInfo != nullptr)
            this->providerInfo = new ProviderInfo{

                solution.providerInfo->vehicle_number,
                solution.providerInfo->warehouse_x,
                solution.providerInfo->warehouse_y,                
                solution.providerInfo->truck_capacity,
                solution.providerInfo->due_date               
            };

        this->providerInfo->due_date = solution.providerInfo->due_date;

        this->routes.clear();
        this->routes.shrink_to_fit();
        for(auto & route : solution.routes){
            
            Route * new_route = new Route(*this->providerInfo);
            *new_route = *route.get();//copy constructor of route class
            this->routes.push_back(std::move(std::make_unique<Route>(*new_route)));
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

    float getTotalDistance();
    float getTotalTime();
    unsigned int getNOfRoutes();

    void clearSolution();
    void addProviderInfo(ProviderInfo & provInfo);

    float objectiveFunction(float route_coeff=1000.0f, float distance_coeff=100.0f, float time_coeff=100.f);
};

#endif