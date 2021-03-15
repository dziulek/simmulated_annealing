#include "simmulated_annealing.hpp"


//the simplest algorithm for finding initial solution for the problem
CRPTW_Solution & SimmulatedAnnealing::greedy_init_alg(){

    //clear current solution
    //to do

    if(customers.size() == 0){
        //throw exception
        //to do
    }


    //check basic constraints and feasibility of customers/data
    //to do
    std::vector<bool> visited_customers(0, customers.size());
    unsigned int n_remain = customers.size();

    std::vector<Customer *> cust_to_visit;
    for(auto & customer : customers){
        cust_to_visit.push_back(&customer);
    }

    Route * current_route = &solution->addRoute();

    while(n_remain > 0){
        
        int n = current_route->getSizeOfroute();
        //search for best fitting customer (beginTime criteria)
        std::sort(cust_to_visit.begin(), cust_to_visit.end(), [&](Customer *c1, Customer *c2){
            return current_route->newBeginTime((*current_route)[n - 1], *c1) <
                                current_route->newBeginTime((*current_route)[n - 1], *c2);
        });

        bool picked = false;

        for(auto & customer : cust_to_visit){

            if(customer->l >= current_route->newBeginTime((*current_route)[n - 1], *customer)){
                
                current_route->appendCustomer(*customer);
                n_remain --;
                picked = true;
                break;
            }
        }

        if (picked == false){

            current_route = &solution->addRoute();
        }
        
    }

    return *solution;
}