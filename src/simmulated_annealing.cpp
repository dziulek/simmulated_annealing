#include "simmulated_annealing.hpp"


CRPTW_Solution & SimmulatedAnnealing::findInitSolution(const char* alg_name){
    if(customers.size() == 0){
        //to do
        //throw exception
    }
    if(providerInfo == nullptr){
        //to do
        //throw exception
    }
    //to do -> implementation of more algorithms

    return greedy_init_alg();
}

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

            if(customer->l + eps >= current_route->newBeginTime((*current_route)[n - 1], *customer) 
                            && current_route->getRemainingCapacity() + eps >= customer->q){
                
                bool b = current_route->appendCustomer(*customer);
                if(b == false)
                    continue;
                customer = std::move(cust_to_visit.back());
                cust_to_visit.pop_back();
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
    ProviderInfo * temp_prov_info = new ProviderInfo();
    input.open(fileName.c_str(), std::ios::in);

    if(!input.is_open()){

        std::cout << "Błąd otwarcia pliku\n";
        return -1;
    }
    //clear current dataset and solution
    this->customers.clear();
    this->customers.shrink_to_fit();
    if(solution == nullptr){
        this->solution = new CRPTW_Solution();
    }
    this->solution->clearSolution();

    enum State{DATASETANAME, CUSTOMER, VEHICLE};
    State state = DATASETANAME;
    std::string line, temp;

    input >> this->dataSetName;
    input >> temp;
    if(stringToLower(temp) != "vehicle"){
        //to do
        std::cout << temp << std::endl;
        return -1;
    }
    input >> temp >> temp;
    input >> temp_prov_info->vehicle_number >> temp_prov_info->truck_capacity;
    input >> temp;
    if(stringToLower(temp) != "customer"){
        //to do 
        return -1;
    }
    getline(input, temp);
    getline(input, temp);
    int word_num;
    int it;
    std::vector<float> words(7);
    while(getline(input, line)){
        if(line.length() == 0)
            continue;
        it = 0;
        word_num = 0;
        while(it < line.length()){
            if(line[it] == ' ' || line[it] == '\t'){
                it ++;continue;
            }
            int next_space = line.find_first_of(" \t", it);
            words[word_num] = std::stof(line.substr(it, next_space - it));
            it = next_space;
            word_num++;
        }
        if(words[0] == 0){//its magazine
            temp_prov_info->warehouse_x = words[1];
            temp_prov_info->warehouse_y = words[2];
            temp_prov_info->due_date = words[5];
        }
        else {
            customers.emplace_back((int)words[0], words[1], words[2], words[3], words[4], words[5], words[6]);
        }
    }
    if(this->providerInfo != nullptr)
        delete this->providerInfo;
    this->providerInfo = temp_prov_info;

    this->solution->addProviderInfo(*this->providerInfo);

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

std::string SimmulatedAnnealing::stringToLower(std::string & s){
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
    [](unsigned char c){ return std::tolower(c); });
    return out;
}