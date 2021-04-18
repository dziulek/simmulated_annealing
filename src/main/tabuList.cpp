#include "tabuList.hpp"

void TabuList::actualizeTabuList(){

    if(this->time > MAXVALUE){
        
        for(auto & customer : tabu){

            for(auto & value : customer){
                
                value = std::max(0, value - time);
            }
        }
        //reset history
        time = 0;        
    }

}

void TabuList::incrementTime(){
    this->time ++;
}

bool TabuList::isValidMove(const int custA, const int routeA, const int custB, const int routeB){

    if(this->tabu[custA][routeA] <= time || this->tabu[custB][routeB] <= time) return true;

    return false;
}

void TabuList::resetTime(){

    this->time = 0;
}

void TabuList::recordOperation(const int custA, const int routeA, const int custB, const int routeB){

    //now it is simplified
    this->tabu[custA][routeA] = this->time + tabuPeriod;//customer A can't return to route A in tabuPeriod time
    this->tabu[custB][routeB] = this->time + tabuPeriod;//customer B cant't return to route B in tabuPeriod time
}