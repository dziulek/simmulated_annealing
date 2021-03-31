#include "route.hpp"

float Route::newBeginTime(const unsigned int i){

    return std::max(this->route[i].customer->e,
                    this->route[i - 1].beginTime + Customer::dist(*route[i - 1].customer, *route[i].customer) + this->route[i -1].customer->d);
}
float Route::newBeginTime(const routeCustomer &prec, const Customer &succ){

    return std::max(succ.e, prec.beginTime + Customer::dist(succ, *prec.customer) + prec.customer->d);
}

float Route::newBeginTime(const Customer &prec, const Customer &succ, const float beginTimePrec){

    return std::max(succ.e, beginTimePrec + Customer::dist(succ, prec) + prec.d);
}

float Route::calcWaitingTime(const unsigned int i){

    float arrival = this->route[i - 1].beginTime + route[i - 1].customer->d + Customer::dist(*route[i].customer, *route[i - 1].customer);

    return std::max(0.0f, this->route[i].customer->e - arrival);
}

float Route::calcWaitingTime(const routeCustomer &prec, const Customer &succ){

    float arrival = prec.beginTime + prec.customer->d + Customer::dist(*prec.customer, succ);

    return std::max(0.0f, succ.e - arrival);
}

bool Route::checkIfPossiblePushRoute(Route &r, const unsigned int _i, float &routeOffsetIn, float &routeOffsetOut){


    if( abs(routeOffsetIn) <= eps){

        routeOffsetOut = 0.0f;
        return 1;
    }

    if(routeOffsetIn > 0){
        
        float pushRoute = routeOffsetIn;

        for(int i = _i + 1; i < r.route.size() - 2; i++){

            if( r[i].beginTime + pushRoute > r[i].customer->l){
                
                return 0;//cannot push customer due to his window constraints
            }

            pushRoute = std::max(0.0f, pushRoute - r[i + 1].waitingTime);
        }
        if(r[r.getSizeOfroute() - 1].beginTime + pushRoute > r[r.getSizeOfroute() - 1].customer->l){
            
            return 0;
        }
        routeOffsetOut = pushRoute;
    }
    else {
        
        float pushRoute = routeOffsetIn, timeOverflow;

        for(int i = _i + 1; i < r.getSizeOfroute() -1; i++){

            timeOverflow = r[i].beginTime - r[i].customer->e;

            if(timeOverflow + pushRoute <= eps) pushRoute = -timeOverflow;
            if(abs(pushRoute) < eps){

                routeOffsetOut = 0;
                return 1;
            }
        }
        routeOffsetOut = pushRoute;
        return 1;
    }
    return 0;

}

bool Route::checkIfCanReturnInTime(){

    float b = this->newBeginTime(this->route.back(), *route[0].customer);

    if(b > route[0].customer->l + eps)
        return false;

    return true;
}

bool Route::appendCustomer(Customer &c){

    float d = this->getLastCustomer().distance + Customer::dist(c, *route.back().customer);
    float b = this->newBeginTime(this->getLastCustomer(), c);

    float wTime = calcWaitingTime(this->getLastCustomer(), c);

    if(b > c.l + eps){
        
        throw WindowConstraintException(c.l, b, &c);
    }

    if(this->totalCapacity + c.q > eps + MAX_CAPACITY){
        
        throw CapacityException(MAX_CAPACITY, &c, totalCapacity);
    }

    float returnTime = Route::newBeginTime(c, *this->route.back().customer, b);
    if(returnTime > magazine->l + eps){

        throw WindowConstraintException(this->route.back().customer->l, returnTime, this->route.back().customer);
    }

    this->totalCapacity += c.q;
    this->route.back().beginTime = returnTime;
    this->route.back().distance += (d + Customer::dist(*this->route.back().customer, c));

    this->route.insert(route.begin() + route.size() - 1 , {&c, b, d, wTime});

    return true;
}

routeCustomer & Route::operator[](std::size_t _i){

    if(_i < 0 || _i >= route.size()){
        throw std::out_of_range("Try to access route customer at index " + std::to_string(_i) + ",\nsize of route: " + std::to_string(this->route.size()));
    }
    return this->route[_i];
}

routeCustomer & Route::getLastCustomer(){

    return this->route[route.size() - 2];
}

const routeCustomer & Route::operator<=(const Route & r){
    //needs objective function
}

const routeCustomer & Route::operator>=(const Route & r){
    //objective function needed
}

float Route::getTimeCost() const{

    return this->route.back().beginTime;
}

bool Route::insertCustomerIntoRoute(Customer &c, const unsigned int i){

    const unsigned int _i = i;

    //to do
    //check index
    if(_i < 1 || _i > route.size() - 1){
        throw std::out_of_range("try to access customer at index: " + std::to_string(_i));
    }

    if(c.q + this->totalCapacity > MAX_CAPACITY){
        
        throw CapacityException(MAX_CAPACITY, &c, this->totalCapacity);
    }

    float newbtime = newBeginTime(this->route[_i - 1], c);
    float newDistanceForCustomer = this->route[_i - 1].distance + Customer::dist(*this->route[_i].customer, c);
    
    float distInsSucc = Customer::dist(*this->route[_i].customer, c);
    float distPrecSucc = Customer::dist(*this->route[_i - 1].customer, *this->route[_i].customer);

    float distanceDiff = newDistanceForCustomer + distInsSucc - distPrecSucc;

    this->route.insert(this->route.begin() +_i, {&c, newbtime, newDistanceForCustomer});

    float pushCustomers = newBeginTime(_i + 1) - this->route[_i + 1].beginTime;

    this->route[_i].waitingTime = calcWaitingTime(_i + 1);

    for(int j = _i + 1; j < this->route.size() - 1; j++){

        this->route[j].beginTime += pushCustomers;
        if(this->route[j].beginTime > this->route[j].customer->l){
            
            throw WindowConstraintException(this->route[j].customer->l, this->route[j].beginTime, this->route[j].customer);
        }
        if(abs(pushCustomers) < eps)
            break;//nothing to do

        this->route[j].distance += distanceDiff;

        float temp = pushCustomers;

        pushCustomers = std::max(0.0f, pushCustomers - this->route[j + 1].waitingTime);
        route[j + 1].waitingTime = std::max(0.0f, route[j + 1].waitingTime - temp);
    }

    this->route.back().beginTime += pushCustomers;
    this->route.back().distance += distanceDiff;

    return true;
}

bool Route::deleteCustomer(const unsigned int _i){
    
    const unsigned int i = _i;

    //to do 
    //check index range
    if(i < 1 || i > route.size() - 2){
        throw std::out_of_range("try to delete customer at index: " + std::to_string(i));
    }

    this->totalCapacity -= this->route[i].customer->q;

    float diffDistance = Customer::dist(*this->route[i - 1].customer, *this->route[i + 1].customer) -
                        (this->route[i + 1].distance - this->route[i - 1].distance);
    
    this->route.erase(route.begin() + i);
    float pushBack = newBeginTime(i) - this->route[i].beginTime;

    this->route[i].waitingTime = calcWaitingTime(i);

    //slow naive version
    for(int j = i; j < route.size() - 1; j++){

        this->route[j].distance += diffDistance;
        this->route[j].beginTime += pushBack;

        if(abs(pushBack) < eps)
            break;

        float temp = pushBack;

        pushBack = std::max(0.0f, pushBack - this->route[j + 1].waitingTime);
        this->route[j + 1].waitingTime = std::max(0.0f, this->route[j + 1].waitingTime - pushBack);

    }

    this->route.back().beginTime += pushBack;
    this->route.back().distance += diffDistance;

    return true;
    
}

bool Route::checkIfPossibleDeleteInsert(Route &r1, const unsigned int _i, Route &r2, const unsigned int _j, routeImprovement &score){

    if(_i == 0 || _i == r1.getSizeOfroute() -1){
        
        throw std::invalid_argument("cannot delete magazine from route");
    }

    if( r2.getRemainingCapacity() - r1[_i].customer->q < eps ) return 0;

    float offIn_r1, offOut_r1;

    offIn_r1 = Route::newBeginTime(r1[_i - 1], *r1[_i + 1].customer) - r1[_i + 1].beginTime;

    if(Route::checkIfPossiblePushRoute(r1, _i, offIn_r1, offOut_r1)){

        float offIn_r2, offOut_r2;

        float temp = Route::newBeginTime(r2[_j - 1], *r1[_i].customer);
        if(temp > r1[_i].customer->l) return 0;

        offIn_r2 = Route::newBeginTime(r1[_i], *r2[_j].customer) - r2[_j].beginTime;

        if(checkIfPossiblePushRoute(r2, _j, offIn_r2, offOut_r2)){

            score.routeCostDiff = offOut_r1 + offOut_r2;
            float d_r1 = Customer::dist(*r1[_i - 1].customer, *r1[_i + 1].customer) - (r1[_i + 1].distance - r1[_i - 1].distance);
            float d_r2 = (r2[_j + 1].distance - r2[_j - 1].distance) - Customer::dist(*r2[_j - 1].customer, *r2[_j + 1].customer);

            score.routeDistanceDiff = d_r1 + d_r2;

            if(r1.getSizeOfroute() == 3) score.routeNumber = -1;//we delete the last customer so route will be empty
            else score.routeNumber = 0;

            return 1;
        }

    }
    return 0;
}

bool Route::checkIfPossibleSwapBetweenRoutes(Route &r1, const unsigned int _i, Route &r2, const unsigned int _j, routeImprovement &score){

    if(_i == 0 || _i == r1.getSizeOfroute() - 1 || _j == 0 ||  _j == r2.getSizeOfroute() - 1){
        throw std::out_of_range("try to swap magazine between routes");
    }

    if(r1.getRemainingCapacity() - r1[_i].customer->q + r2[_j].customer->q < 0.f) return false;
    if(r2.getRemainingCapacity() - r2[_j].customer->q + r1[_i].customer->q < 0.f) return false;

    float temp_r1, temp_r2;

    temp_r1 = Route::newBeginTime(r1[_i - 1], *r2[_j].customer);
    if(temp_r1 > r2[_j].customer->l) return 0;

    temp_r2 = Route::newBeginTime(r2[_j - 1], *r1[_i].customer);
    if(temp_r2 > r1[_i].customer->l) return 0;

    float offIn_r1, offOut_r1, offIn_r2, offOut_r2;

    offIn_r1 = Route::newBeginTime(*r1[_i - 1].customer, *r2[_j].customer, temp_r1);
    offIn_r2 = Route::newBeginTime(*r2[_j - 1].customer, *r1[_i].customer, temp_r2);

    if(checkIfPossiblePushRoute(r1, _i + 1, offIn_r1, offOut_r1)){

        if(checkIfPossiblePushRoute(r2, _j + 1, offIn_r2, offOut_r2)){

            score.routeCostDiff = offOut_r1 + offOut_r2;

            float d_r1, d_r2;
            d_r1 = Customer::dist(*r1[_i - 1].customer, *r2[_j].customer) 
                    + Customer::dist(*r2[_j].customer, *r1[_i + 1].customer)
                    + (r1[_i + 1 + 1].distance - r1[_i].distance);
            
            d_r2 = Customer::dist(*r2[_j - 1].customer, *r1[_i].customer)
                    + Customer::dist(*r1[_i].customer, *r2[_j + 1].customer)
                    + (r2[_j + 1].distance - r2[_j].distance);

            score.routeDistanceDiff = d_r1 + d_r2;
            score.routeNumber = 0;

            return 1;
        }
    }
    return 0;
}

bool Route::execDeleteInsert(Route &r1, const unsigned int _i, Route &r2, const unsigned int _j){

    try{
        r2.insertCustomerIntoRoute(*r1[_i].customer, _j);

    }catch(const std::exception & e){
        
        std::cout << e.what() << std::endl;
        return false;
    }

    r1.deleteCustomer(_i);

    return true;
}

bool Route::execSwapBetweenRoutes(Route &r1, const unsigned int _i, Route &r2, const unsigned int _j){

    Customer * c1 = r1[_i].customer;
    Customer * c2 = r2[_j].customer;

    r1.deleteCustomer(_i);
    r2.deleteCustomer(_j);

    try
    {
        r1.insertCustomerIntoRoute(*c2, _i);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
    
    try{
        r2.insertCustomerIntoRoute(*c1, _j);
    }
    catch(const std::exception & e){
        
        std::cout << e.what() << std::endl;
    }
}
