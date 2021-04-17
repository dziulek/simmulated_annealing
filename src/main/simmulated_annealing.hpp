#ifndef SIMMULATED_ANNEALING_HPP
#define SIMMULATED_ANNEALONG_HPP

#include "customer.hpp"
#include "route.hpp"
#include "solution.hpp"
#include "tabuList.hpp"
#include "constants.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <chrono>
#include <pthread.h>

// #ifdef GRAPH_HPP
// pthread_mutex_t annealing_operation_mutex = PTHREAD_MUTEX_INITIALIZER;
// bool GRAPHICAL_INTERFACE = true;
// #endif

class SimmulatedAnnealing {

private:

    CRPTW_Solution * solution;
    std::vector<Customer> customers;

    ProviderInfo * providerInfo = nullptr;
    std::string dataSetName;
    std::vector<std::vector<int>> tabuList;

    //temporarily redundant
    Customer * magazine = nullptr;

    //metaheuristic parameters
    float pInit; //initial probability value
    float TEMPINIT; //initial temperature
    float RATIO;//
    float epochLen;//length of the epoch
    float MAX_TIME;//
    float MIN_PERCENT;

    void greedy_init_alg();
    void setParams(const float avgCostIncrease);
    bool terminateSearch();
    bool nextMove(int & CustA, int & routeA, int &custB, int & routeB, TabuList & tabuList, int & moveNumber);//algorithm for searching next move
    void routeRearange(std::vector<int> & indexes);

    float probabilityThreshold();

    std::string stringToLower(std::string & s);

    bool compareCustomers(Route * route, const Customer * c1, const Customer * c2);

    

public:

    SimmulatedAnnealing(){
        solution = new CRPTW_Solution(); 
        providerInfo = nullptr;
    }
    virtual ~SimmulatedAnnealing(){
        if(solution != nullptr)
            delete solution;
        if(providerInfo != nullptr)
            delete providerInfo;
        if(magazine != nullptr)
            delete magazine;
    }

    int parseDataFromFile(std::string fileName);
    void findInitSolution(const char* alg_name);
    void runAlgorithm(std::string initAlg="greedy", bool threadSafe=false);

    Customer & getCustomer(unsigned int index);
    ProviderInfo & getProviderInfo();
    unsigned int getCustomerNumber();
    CRPTW_Solution * getSolution(){return this->solution;}

    std::vector<Customer> & getCustomers(){return this->customers;}
    Customer & getMagazine(){ return *this->magazine;}
    static const std::string getPathToWorkspaceFolder();

    friend class Graph;
};

#endif