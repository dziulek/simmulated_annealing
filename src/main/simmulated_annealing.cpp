#include "simmulated_annealing.hpp"

const std::string SimmulatedAnnealing::getPathToWorkspaceFolder(){

    char pathName[FILENAME_MAX];
    getcwd(pathName, FILENAME_MAX);

    std::cerr << pathName << std::endl;

    std::string directory(pathName);
    directory += "/";

    auto pos = directory.rfind("/simmulated_annealing/");
    if(pos == std::string::npos){
        std::cerr << "cannot find directory" << std::endl;
        return "";
    }

    directory = directory.substr(0, pos + 1);
    directory += "simmulated_annealing/";
    return directory;
}

void SimmulatedAnnealing::findInitSolution(const char* alg_name, bool threadSafe){

    this->status = INIT_SOLUTION;

    if(customers.size() == 0){
        //to do
        //throw exception
    }
    if(providerInfo == nullptr){
        //to do
        //throw exception
    }
    //to do -> implementation of more algorithms

    greedy_init_alg(threadSafe);
}

//the simplest algorithm for finding initial solution for the problem
void SimmulatedAnnealing::greedy_init_alg(bool threadSafe){

    //clear current solution
    this->solution->clearSolution();

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

    LOCK(annealing_operation_mutex, threadSafe)
        Route * current_route = &solution->addRoute();
    UNLOCK(annealing_operation_mutex, threadSafe)


    for(int i = 0; i < cust_to_visit.size() -2; i++)
        for(int j = i + 1; j < cust_to_visit.size() -1; j++)
            Customer::dist(*cust_to_visit[i], *cust_to_visit[j]);
    
    while(n_remain > 0){
        
        int n = current_route->getSizeOfroute();
        // search for best fitting customer (beginTime criteria)
        std::sort(cust_to_visit.begin(), cust_to_visit.end(), [&](const Customer * c1, const Customer * c2){
            return current_route->newBeginTime(current_route->getLastCustomer(), *c1) < 
                        current_route->newBeginTime(current_route->getLastCustomer(), *c2);
        });

        bool picked = false;

        SET_LATENCY(LATENCY_MICROSECONDS, threadSafe)
        LOCK(annealing_operation_mutex, threadSafe)
        


        for(auto & customer : cust_to_visit){

            if(customer->l + eps >= current_route->newBeginTime(current_route->getLastCustomer(), *customer) 
                            && current_route->getRemainingCapacity() + eps >= customer->q){
                bool b;
                try{


                        b = current_route->appendCustomer(*customer);

                    customer = std::move(cust_to_visit.back());
                    cust_to_visit.pop_back();
                    n_remain --;
                    picked = true;
                    // sleep(1);
                    break;

                }catch(WindowConstraintException & e){

                    continue;
                }

            }
        }

        if (picked == false){
                current_route = &solution->addRoute();

        }
        
        UNLOCK(annealing_operation_mutex, threadSafe)        
    }
}

bool SimmulatedAnnealing::compareCustomers(Route * route, const Customer * c1, const Customer * c2){

    const routeCustomer * last = &route->getLastCustomer();
    return route->newBeginTime(*last, *c1) < route->newBeginTime(*last, *c2);
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
    
    std::cout << "Loading data...\n";

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
            if(line[it] == ' ' || line[it] == '\t' || line[it] == '\r'){
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
    if(customers.back().id == (customers.end() - 2)->id)
        customers.pop_back();

    if(this->providerInfo != nullptr)
        delete this->providerInfo;
    this->providerInfo = temp_prov_info;

    if(this->magazine != nullptr)
        delete this->magazine;

    this->magazine = new Customer(0, providerInfo->warehouse_x, providerInfo->warehouse_y, 0, 0, providerInfo->due_date, 0);

    this->solution->addProviderInfo(*this->providerInfo);
    

    input.close();

    std::cout << "Data Succesfully loaded.\n" + std::to_string(customers.size()) + " customers.\n";

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

CRPTW_Solution * SimmulatedAnnealing::getBestSolution(){
    
    return (status == METAHEURISTIC ? this->bestSolution : this->solution);
}

std::string SimmulatedAnnealing::stringToLower(std::string & s){
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(),
    [](unsigned char c){ return std::tolower(c); });
    return out;
}

void SimmulatedAnnealing::runAlgorithm(std::string initAlg, bool threadSafe){

    //check if all resources have been loaded
    if(customers.size() == 0){
        //to do
    }
    if(providerInfo == nullptr){
        //to do
    }

    //clear previous output
    if(this->solution != nullptr){
        delete this->solution;
        this->solution = new CRPTW_Solution(*this->providerInfo);
    }

    // this->solution = new CRPTW_Solution(*this->providerInfo);
    // this->solution = 
    greedy_init_alg(threadSafe);

    if(!CRPTW_Solution::isValid(*this->solution))
        return;
    // std::cerr << "after greedy" << std::endl;

    CRPTW_Solution * current_solution = solution;


    bestSolution = new CRPTW_Solution(*current_solution);

    this->status = METAHEURISTIC;

    // return (void)"siemano";

    int random_iterations = 0;
    int continuous_overlap = 0;
    float delta, averageDelta=0.f;
    int optimumConstCounter = 0, noImprovementIteration = 0;
    int __counter = 0;
    float FINDDIVISOR = 500000;
    float temperature = this->TEMPINIT;

    int moveType;

    int rejectedIterations = 0;

    int custA, custB, routeA, routeB;


    //move_number: if 0 then delete insert is considered, otherwise swap between routes
    unsigned int move_number, route_i, route_j, cust_i, cust_j;

    routeImprovement routeImprv;

    while(continuous_overlap < customers.size() * customers.size() && 
                    random_iterations < N_RANDOM_ITERATIONS && solution->getNOfRoutes() > 2){

        move_number = rand()%Route::NMOVES;
        route_i = rand()%current_solution->getNOfRoutes();
        route_j = rand()%current_solution->getNOfRoutes();

        while(route_j == route_i)
            route_j = rand()%current_solution->getNOfRoutes();

        cust_i = rand()%(current_solution->getRoute(route_i).getSizeOfroute() - 2) + 1;
        cust_j = rand()%(current_solution->getRoute(route_j).getSizeOfroute() - 2) + 1;

        if(move_number == Route::DELETE_INSERT){

            if(Route::checkIfPossibleDeleteInsert(current_solution->getRoute(route_i), cust_i, 
                                        current_solution->getRoute(route_j), cust_j, routeImprv)){
                
                SET_LATENCY(LATENCY_MICROSECONDS, threadSafe)                
                LOCK(annealing_operation_mutex, threadSafe)


                random_iterations++;
                Route::execDeleteInsert(
                    current_solution->getRoute(route_i),
                    cust_i, 
                    current_solution->getRoute(route_j),
                    cust_j
                );
                
                UNLOCK(annealing_operation_mutex, threadSafe)
                
                delta = routeImprv.objectiveFunction();
                averageDelta += abs(delta);
                continuous_overlap = 0;
            }
            else continuous_overlap ++;
        }
        else {

            if(Route::checkIfPossibleSwapBetweenRoutes(
                current_solution->getRoute(route_i), cust_i,
                current_solution->getRoute(route_j), cust_j, routeImprv
            )){
                SET_LATENCY(LATENCY_MICROSECONDS, threadSafe)
                LOCK(annealing_operation_mutex, threadSafe)
                

                random_iterations++;
                Route::execSwapBetweenRoutes(
                    current_solution->getRoute(route_i), cust_i,
                    current_solution->getRoute(route_j), cust_j
                );
                
                UNLOCK(annealing_operation_mutex, threadSafe)

                delta = routeImprv.objectiveFunction();
                averageDelta += abs(delta);
                continuous_overlap = 0;
            }
            else continuous_overlap ++;
        }
    }

    if(averageDelta < eps){
        //pick default value based on some heuristic
        averageDelta = 1.02;
    }

    averageDelta /= N_RANDOM_ITERATIONS;

    this->setParams(averageDelta);
    temperature = this->TEMPINIT;

    TabuList tabuList(*current_solution, customers.size());

    auto START = std::chrono::steady_clock::now();

    while(temperature >= (TEMPINIT / FINDDIVISOR) && current_solution->getNOfRoutes() > 2){

        optimumConstCounter = 0;
        __counter = 0;
        rejectedIterations = 0;
        while(optimumConstCounter < this->MAX_TIME && __counter * MIN_PERCENT <= (__counter - rejectedIterations) && solution->getNOfRoutes() > 2
                        && std::chrono::duration<double>(std::chrono::steady_clock::now() - START).count() < 100){
            
            SET_LATENCY(LATENCY_MICROSECONDS, threadSafe)
            LOCK(annealing_operation_mutex, threadSafe)
            
            if(nextMove(custA, routeA, custB, routeB, tabuList, moveType, temperature)){

                if(moveType == Route::DELETE_INSERT){

                    Route::execDeleteInsert(current_solution->getRoute(routeA), custA, current_solution->getRoute(routeB), custB);
                    if(current_solution->getRoute(routeA).getSizeOfroute() == 2){
                        current_solution->deleteRoute(routeA);
                    }
                }
                else {

                    Route::execSwapBetweenRoutes(current_solution->getRoute(routeA), custA, current_solution->getRoute(routeB), custB);
                }

                // if(!CRPTW_Solution::isValid(*current_solution))
                //     return;

                // std::cerr << current_solution->getNOfRoutes() << " " << current_solution->getTotalTime() << " " << current_solution->getTotalDistance() << std::endl;

                //check if found best solution
                if(current_solution->objectiveFunction() < bestSolution->objectiveFunction()){
                    
                    // LOCK(annealing_operation_mutex, threadSafe)
                    delete bestSolution;
                    bestSolution = new CRPTW_Solution(*current_solution);
                    // UNLOCK(annealing_operation_mutex, threadSafe)
                    optimumConstCounter = 0;
                }
                else optimumConstCounter ++;
            }
            else {

                rejectedIterations ++;
                optimumConstCounter ++;
                
            }
            __counter ++;
            tabuList.incrementTime();

            UNLOCK(annealing_operation_mutex, threadSafe)            
        }

        temperature *= this->RATIO;
        tabuList.actualizeTabuList();

        // std::cerr << "Temperature: " << temperature << std::endl;
    }
    LOCK(annealing_operation_mutex, threadSafe)
    delete this->solution;
    this->solution = new CRPTW_Solution(*bestSolution);
    UNLOCK(annealing_operation_mutex, threadSafe)

    std::cerr << "Algorithm stopped" <<std::endl;

    status = NONE;
    delete bestSolution;
}

void SimmulatedAnnealing::setParams(const float avgCostIncrease){
    
    this->pInit = 0.95;
    this->TEMPINIT = -avgCostIncrease / log(this->pInit);
    this->epochLen = 1e5;
    this->MIN_PERCENT = 0.01;
    this->RATIO = 0.95;
    this->MAX_TIME = 2 * customers.size();
}

bool SimmulatedAnnealing::terminateSearch(){


}

bool SimmulatedAnnealing::nextMove(int & __custA, int & __routeA, int & __custB, int & __routeB, TabuList & tabuList, int & moveNumber, float temperature){

    float d, p;
    routeImprovement routeImpr;
    float k1 = 0.3, k2 = 0.3;

    std::vector<int> route1Indexes, route2Indexes;
    route1Indexes.resize(this->solution->getNOfRoutes());
    route2Indexes.resize(this->solution->getNOfRoutes());
    for(int i = 0; i < this->solution->getNOfRoutes(); i++){

        route1Indexes[i] = i;
        route2Indexes[i] = i;
    }

    this->routeRearange(route1Indexes);
    this->routeRearange(route2Indexes);

    for(auto & i : route1Indexes){

        for(auto & j : route2Indexes){

            //routes have to be different
            if(i == j)
                continue;
            
            for(int custA = 1; custA < solution->getRoute(i).getSizeOfroute() - 1; custA ++){

                for(int custB = 1; custB < solution->getRoute(j).getSizeOfroute() - 1; custB ++){

                    moveNumber = rand()%2;
                    
                    __custA = custA;
                    __custB = custB;
                    __routeA = i;
                    __routeB = j;

                    if(moveNumber == 0){
                        //consider delete insert operation
                        if(Route::checkIfPossibleDeleteInsert(
                            solution->getRoute(i), custA, solution->getRoute(j), custB, routeImpr
                        )){
                            
                            if(tabuList.isValidMove(custA, i, custB, j)){

                                d = routeImpr.objectiveFunction();
                                if(d < eps){

                                    tabuList.recordOperation(custA, i, custB, j);
                                    return true;                                    
                                }
                                //give a second chance with some probability 
                                p = secondChanceProbability(d, temperature) * (1.f - k1) * (1.f - k2);

                                if(this->probabilityThreshold() > p){

                                    tabuList.recordOperation(custA, i, custB, j);
                                    return true;
                                }
                            }      
                        }//try reverse insertion
                        else if(Route::checkIfPossibleDeleteInsert(
                            solution->getRoute(j), custB, solution->getRoute(i), custA, routeImpr
                        )){
                            std::swap(__custA, __custB);
                            std::swap(__routeA, __routeB);

                            if(tabuList.isValidMove(custB, j, custA, i)){
                                
                                d = routeImpr.objectiveFunction();
                                if(d < eps){

                                    tabuList.recordOperation(custB, j, custA, i);
                                    return true;                                    
                                }

                                //give a second chance with some probability
                                p = secondChanceProbability(d, temperature) * (1.f - k1) * (1.f - k2);

                                if(this->probabilityThreshold() > p){

                                    tabuList.recordOperation(custB, j, custA, i);
                                    return true;
                                }
                            }                                 
                        }
                    }
                    else {
                        //consider swap operation
                        if(Route::checkIfPossibleSwapBetweenRoutes(
                            solution->getRoute(i), custA, solution->getRoute(j), custB, routeImpr
                        )){
                            d = routeImpr.objectiveFunction();
                            if(tabuList.isValidMove(custA, i, custB, j)){
                                
                                if(d < eps){//route improvmement

                                    tabuList.recordOperation(custA, i, custB, j);
                                    return true;
                                }

                                //give a second chance with some probability
                                p = secondChanceProbability(d, temperature) * (1.f - k1) * (1.f - k2);

                                if(this->probabilityThreshold() > p){

                                    tabuList.recordOperation(custA, i, custB, j);
                                    return true;
                                }      
                            }               
                        }
                    }
                }
            }
        }
    }

    return false;
}

void SimmulatedAnnealing::routeRearange(std::vector<int> & indexes){
 
    std::random_shuffle(indexes.begin(), indexes.end());
}

float SimmulatedAnnealing::probabilityThreshold(){

    return static_cast<float>(rand())/RAND_MAX;
}

float SimmulatedAnnealing::secondChanceProbability(float delta, float temperature){

    return exp(-delta / temperature);
}