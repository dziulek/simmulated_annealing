#include "route.hpp"


float Route::newBeginTime(const unsigned int i){

    return std::max(this->route[i].customer->e,
                    this->route[i - 1].beginTime + Customer::dist(*route[i - 1].customer, *route[i].customer));
}

float Route::newBeginTime(const routeCustomer &prec, const Customer &succ){

    return std::max(succ.e, prec.beginTime + Customer::dist(succ, *prec.customer));
}

float Route::calcWaitingTime(const unsigned int i){

    float arrival = this->route[i - 1].beginTime + route[i - 1].customer->d + Customer::dist(*route[i].customer, *route[i - 1].customer);

    return std::max(0.0f, this->route[i].customer->e - arrival);
}

float Route::calcWaitingTime(const routeCustomer &prec, const Customer &succ){

    float arrival = prec.beginTime + prec.customer->d + Customer::dist(*prec.customer, succ);

    return std::max(0.0f, succ.e - arrival);
}

bool Route::checkIfPossibleDeletionInsertion(const Route &r, const Customer &c, const unsigned int _i, float &routeOffsetIn, float &routeOffsetOut){

    if( abs(routeOffsetIn) <= eps){

        routeOffsetOut = 0.0f;
        return 1;
    }

    if(routeOffsetIn > 0){
        
        float pushRoute = routeOffsetIn;

        for(int i = _i + 1; i < r.getSizeOfroute() - 2; i++){

            if( r[i].beginTime + pushRoute > r[i].customer->l) return 0;//cannot push customer due to his window constraints

            pushRoute = std::max(0.0f, pushRoute - r[i + 1].waitingTime);
        }
        if(r[i].beginTime + pushRoute > r[i].customer->l) return 0;
        routeOffsetOut = pushRoute;
    }
    else {
        
        float pushRoute = routeOffsetIn, timeOverflow;

        for(int i = _i + 1; i < r.getSizeOfroute() -1; i++){

            timeOverflow = r[i].beginTime - r[i].customer->e;

            if(timeOverflow + pushRoute <= eps) pushRoute = -timeOverflow;
            if(abs(pushRoute) < epsilon){

                routeOffsetOut = 0;
                return 1;
            }
        }
        routeOffsetOut = pushRoute;
        return 1;
    }

    return 0;

}



bool Route::appendCustomer(Customer &c){

    float d = this->route.back().distance + Customer::dist(c, *route.back().customer);
    float b = this->newBeginTime(this->route.back(), c);

    float wTime = calcWaitingTime(this->route.back(), c);

    this->route.push_back({&c, b, d, wTime});

    return true;
}

const routeCustomer & Route::operator[](std::size_t _i) const {

    return this->route[_i];
}

const routeCustomer & Route::operator<=(const Route & r){
    //objective function needed
}

const routeCustomer & Route::operator>=(const Route & r){
    //objective function needed
}

bool Route::insertCustomerIntoRoute(Customer &c, const unsigned int i){

    float newbtime = newBeginTime(this->route[i], c);
    float newDistanceForCustomer = this->route[i].distance + Customer::dist(*this->route[i].customer, c);
    
    float distInsSucc = Customer::dist(*this->route[i].customer, c);
    float distPrecSucc = Customer::dist(*this->route[i-1].customer, *this->route[i].customer);

    float distanceDiff = newDistanceForCustomer + distInsSucc - distPrecSucc;

    this->route.insert(this->route.begin() + i, {&c, newbtime, newDistanceForCustomer});

    this->route[i + 1].beginTime = newBeginTime(i + 1);

    float pushCustomers = this->route[i + 1].beginTime - this->route[i].beginTime;

    this->route[i + 1].waitingTime = calcWaitingTime(i + 1);

    for(int j = i + 1; j < this->route.size() - 1; j++){

        this->route[j].beginTime = newBeginTime(j);
        this->route[j].distance += distanceDiff;

        float temp = pushCustomers;

        pushCustomers = std::max(0.0f, pushCustomers - this->route[j + 1].waitingTime);
        route[j + 1].waitingTime = std::max(0.0f, route[j + 1].waitingTime - pushCustomers);
    }

    this->route.back().beginTime += pushCustomers;
    this->route.back().distance += distanceDiff;

    return true;
}

bool Route::checkIfPossibleDeleteInsert(const Route &r1, const unsigned int _i, const Route &r2, const unsigned int _j, const routeImprovement &score){

    if(r1[_i].customer->id == 0) return 0;
    if( int(r2.getRemainingCapacity) == 0 ) return 0;


}


