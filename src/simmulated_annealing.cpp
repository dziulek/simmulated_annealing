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

int SimmulatedAnnealing::parseDataFromFile(std::string fileName){

    std::fstream input;
    input.open(fileName, std::ios::in);

    if(!input.good()){

        std::cout << "Błąd otwarcia pliku\n";
        return -1;
    }
    //clear current dataset and solution
    this->customers.clear();
    this->customers.shrink_to_fit();
    this->solution->clearSolution();

    input >> this->dataSetName;
    
    std::string helper;

    input >> helper >> helper >> helper;

    ProviderInfo temp_prov_info;

    input >> temp_prov_info.vehicle_number >> temp_prov_info.truck_capacity;

    input >> helper;

    getline(input, helper);

    Customer temp_cust(0, 0, 0, 0, 0, 0);

    while(!input.eof()){

        input >> temp_cust.id;
        input >> temp_cust.x;
        input >> temp_cust.y;
        input >> temp_cust.q;
        input >> temp_cust.e;
        input >> temp_cust.l;
        input >> temp_cust.d;

        this->customers.push_back(temp_cust);
    }

    //ugly check for now
    if(customers.back().id == customers[customers.size() - 2].id){

        this->customers.pop_back();
    }

    input.close();

    return 1;
}

Customer & SimmulatedAnnealing::getCustomer(unsigned int index){

    //index validation
    //to do
    return this->customers[index];
}

ProviderInfo & SimmulatedAnnealing::getProviderInfo(){

    //check if providerInfo is not null
    //to do
    return *providerInfo;
}

unsigned int SimmulatedAnnealing::getCustomerNumber(){

    return this->customers.size();
}