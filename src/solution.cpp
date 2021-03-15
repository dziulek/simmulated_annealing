#include "solution.hpp"

Route & CRPTW_Solution::getRoute(unsigned int i){

    //check index to do
    return *this->routes[i].get();
}

Route & CRPTW_Solution::addRoute(){

    Route * route = new Route(this->providerInfo->truck_capacity);
    std::unique_ptr<Route> temp_ptr = std::make_unique<Route>(*route);
    this->routes.push_back(std::move(temp_ptr));
    this->nOfRoutes++;

    return *route;
}

void CRPTW_Solution::deleteRoute(unsigned int i){

    //check index to do
    this->routes.pop_back();

    this->nOfRoutes --;
    this->totalDistance -= (*routes[i].get())[routes[i].get()->getSizeOfroute() - 1].distance;
    this->totalTime -= this->routes[i].get()->getRouteCost();

    this->routes[i] = std::move(this->routes.back());
}

float CRPTW_Solution::getTotalDistance(){
    
    float sum = 0;

    for(auto & route : routes){
        
        sum += (*route.get())[route.get()->getSizeOfroute() - 1].distance;
    }
    this->totalDistance = sum;

    return this->totalDistance;
}

float CRPTW_Solution::getTotalTime(){

    float sum = 0;
    for(auto & route : routes){

        sum += route.get()->getTimeCost();
    }

    this->totalTime = sum;
    return this->totalTime;
}

unsigned int CRPTW_Solution::getNOfRoutes(){

    return this->nOfRoutes;
}