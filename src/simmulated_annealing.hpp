#ifndef SIMMULATED_ANNEALING_HPP
#define SIMMULATED_ANNEALONG_HPP

#include "customer.hpp"
#include "route.hpp"
#include "solution.hpp"
#include <vector>
#include <string>

class SimmulatedAnnealing {

private:

    CRPTW_Solution * solution;
    std::vector<Customer> customers;

public:

    SimmulatedAnnealing(){
        solution = new CRPTW_Solution();
    }
    virtual ~SimmulatedAnnealing(){}

    int parseDataFromFile(const std::fstream & file) const;
    CRPTW_Solution & findInitSolution(std::string alg_name);

};

#endif