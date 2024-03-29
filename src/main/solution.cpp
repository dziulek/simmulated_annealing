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

float CRPTW_Solution::getTotalDistance() const{
    
    float sum = 0;

    for(auto & route : routes){
        
        sum += route->getRouteCost();
    }

    return sum;
}

float CRPTW_Solution::getTotalTime() const{

    float sum = 0;
    for(auto & route : routes){

        sum += route->getTimeCost();
    }

    return sum;
}

unsigned int CRPTW_Solution::getNOfRoutes() const{

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

bool CRPTW_Solution::isValid(const CRPTW_Solution & solution){

    float current_time = 0, current_distance = 0;
    int truckNo = 0;
    float totalTime = 0.f, totalCost = 0.f;
    float ep = 0.001;
    float rem_capacity;

    for(auto & route : solution.routes){

        if(Route::isValid(*route)){
            totalTime += route->getTimeCost();
            totalCost += route->getRouteCost();
            truckNo++;
        }
        else {
            std::cerr << "error in " << truckNo << " route" << std::endl;
            return false;
        }
    }
    bool out = true;

    if(abs(totalTime - solution.getTotalTime()) > ep){

        out = false;
    }

    if(abs(totalCost - solution.getTotalDistance()) > ep){


        out = false;
    }

    if(truckNo != solution.nOfRoutes){


        out = false;
    }

    if(out == false){
        std::cerr<< "error in total time, expected: " << totalTime << " , found: " << solution.getTotalTime() << std::endl;
        std::cerr << "error in total cost, expected: " << totalCost << " , found: " << solution.getTotalDistance() << std::endl;
        std::cerr << "error in number of routes, expected: " << truckNo << " , found: " << solution.nOfRoutes << std::endl;
    }

    return out;
}

std::ostream & operator<<(std::ostream & os, const CRPTW_Solution & solution){

    os<< solution.getNOfRoutes() << " " << solution.getTotalTime() << " " << solution.getTotalDistance() << std::endl;
    
    os<< std::endl;

    for(auto & route : solution.routes){

        for(int i = 0; i < route->getSizeOfroute(); i++){

            os<< (*route)[i].customer->id << " ";
        }
        os << std::endl;
    }

    return os;
}