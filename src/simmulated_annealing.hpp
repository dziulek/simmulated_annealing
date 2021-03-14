#ifndef SIMMULATED_ANNEALING_HPP
#define SIMMULATED_ANNEALONG_HPP

#include "customer.hpp"
#include "route.hpp"
#include "solution.hpp"
#include <vector>
#include <string>
#include <algorithm>

class SimmulatedAnnealing {

private:

    CRPTW_Solution * solution;
    std::vector<Customer> customers;

    ProviderInfo * providerInfo;

    CRPTW_Solution & greedy_init_alg();

public:

    SimmulatedAnnealing(){
        solution = new CRPTW_Solution();
    }
    virtual ~SimmulatedAnnealing(){}

    int parseDataFromFile(const std::fstream & file) const;
    CRPTW_Solution & findInitSolution(std::string alg_name);

};

#endif