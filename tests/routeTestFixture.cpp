#include "gtest/gtest.h"
#include "../src/main/customer.hpp"
#include "../src/main/route.hpp"

#define OPEN_WINDOW 0
#define CLOSE_WINDOW 10000
#define SERVICE_TIME 10
#define DEMAND 1
#define MAX_CAPACITY 10000

class routeTestFixture : public ::testing::Test{

public:
    
    routeTestFixture(){

        //initialization
        provInfo = new ProviderInfo{0, 0, 0, 10, 10};
        sample_route = new Route(*provInfo);

        for(int i = 0; i < 5; i ++){

            Customer * customer = new Customer(i + 1, float(i + 1), 0.0f, DEMAND, OPEN_WINDOW, CLOSE_WINDOW, SERVICE_TIME);
            this->sample_route->appendCustomer(*customer);
        }
    }

    static void SetUpTestSuite(){
        
    }

    static void TearDownTestSuite(){

    }

    void SetUp(){
        //
    }

    void TearDown(){

    }

    virtual ~routeTestFixture(){
        for(int i = 1; i < sample_route->getSizeOfroute(); i++){
            delete (*sample_route)[i].customer;
        }
        delete sample_route;
        delete provInfo;
    }

    Route * sample_route;
    ProviderInfo * provInfo;

};

TEST_F(routeTestFixture, distance_test){

    for(int i = 0; i < sample_route->getSizeOfroute(); i++){
        ASSERT_FLOAT_EQ(float(i + 1), (*sample_route)[i].distance);
    }
}

TEST_F(routeTestFixture, time_test){

    for(int i = 0; i < sample_route->getSizeOfroute(); i++){
        ASSERT_FLOAT_EQ(float((i) * (SERVICE_TIME + 1) + 1), (*sample_route)[i].beginTime);
    }
}

TEST_F(routeTestFixture, waiting_time){

    for(int i = 0; i < sample_route->getSizeOfroute(); i++){
        ASSERT_FLOAT_EQ(0.0f, (*sample_route)[i].waitingTime);
    }
}