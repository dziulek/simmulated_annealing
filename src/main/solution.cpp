#include "solution.hpp"

Route & CRPTW_Solution::getRoute(unsigned int i){

    //check index to do
    return *this->routes[i];
}

Route & CRPTW_Solution::addRoute(){

    Route * route = new Route(*this->providerInfo);
    // std::unique_ptr<Route> temp_ptr = std::make_unique<Route>(*route);
    this->routes.push_back(route);
    this->nOfRoutes++;

    return *this->routes.back();
}

void CRPTW_Solution::deleteRoute(unsigned int i){

    //check index to do
    this->nOfRoutes --;
    this->totalDistance -= (*routes[i])[routes[i]->getSizeOfroute() - 1].distance;
    this->totalTime -= this->routes[i]->getRouteCost();

    delete routes[i];
    this->routes[i] = this->routes.back();
    this->routes.pop_back();
}

bool CRPTW_Solution::operator<(CRPTW_Solution & solution){

    return this->objectiveFunction() < solution.objectiveFunction();
}

bool CRPTW_Solution::operator>(CRPTW_Solution & solution){

    return this->objectiveFunction() > solution.objectiveFunction();
}

float CRPTW_Solution::objectiveFunction(float route_coeff, float distance_coeff, float time_coeff){

    return this->routes.size() * route_coeff + this->getTotalDistance() * distance_coeff + this->getTotalTime() * time_coeff;
}

float CRPTW_Solution::getTotalDistance(){
    
    // float sum = 0;

    // for(auto & route : routes){
        
    //     sum += (*route)[route->getSizeOfroute() - 1].distance;
    // }
    // this->totalDistance = sum;

    return this->totalDistance;
}

float CRPTW_Solution::getTotalTime(){

    // float sum = 0;
    // for(auto & route : routes){

    //     sum += route->getTimeCost();
    // }

    // this->totalTime = sum;
    return this->totalTime;
}

unsigned int CRPTW_Solution::getNOfRoutes(){

    return this->nOfRoutes;
}

void CRPTW_Solution::clearSolution(){

    for(auto & route : routes)
        delete route;
    this->routes.clear();
    this->routes.shrink_to_fit();

    this->nOfRoutes=0;
    this->totalDistance=0;
    this->totalTime=0;
}

void CRPTW_Solution::addProviderInfo(ProviderInfo & provInfo){

    this->providerInfo = &provInfo;
}