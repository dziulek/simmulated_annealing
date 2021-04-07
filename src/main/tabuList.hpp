#ifndef TABU_LIST_HPP
#define TABU_LIST_HPP

#include "solution.hpp"
#include <vector>
#include <algorithm>

class TabuList{

private:

    const int MAXVALUE = 1e9;
    std::vector<std::vector<int>> tabu;
    int time;//represents history
    int tabuPeriod;

public:

    TabuList(CRPTW_Solution & solution, const int nCustomers){

        this->tabu.resize(nCustomers);
        for(auto & c : tabu){
            c.resize(solution.getNOfRoutes(), 0);
        }
        time = 0;
        
        tabuPeriod = nCustomers / 10;
    }

    virtual ~TabuList(){

    }

    void actualizeTabuList();
    void incrementTime();
    void resetTime();
    bool isValidMove(const int custA, const int routeA, const int custB, const int routeB);
    void recordOperation(const int custA, const int routeA, const int custB, const int routeB);
};

#endif