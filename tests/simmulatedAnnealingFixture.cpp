#include "gtest/gtest.h"
#include "../src/simmulated_annealing.hpp"
#include <iostream>
#include <cmath>

class SimmulatedAnnealingFixture : public ::testing::Test{

public:

    SimmulatedAnnealingFixture(){

        this->simmulatedAnnealing = new SimmulatedAnnealing();
    }

    virtual ~SimmulatedAnnealingFixture(){

        delete simmulatedAnnealing;
    }

    SimmulatedAnnealing * simmulatedAnnealing;
};

TEST_F(SimmulatedAnnealingFixture, parse_from_file){

    int b;
    b = simmulatedAnnealing->parseDataFromFile("fileNotExists.txt");

    ASSERT_EQ(b, -1);

    b = simmulatedAnnealing->parseDataFromFile("../tests/testDataFiles/cvrptw4.txt");

    ASSERT_EQ(b, 1);
    ASSERT_FLOAT_EQ(simmulatedAnnealing->getProviderInfo().vehicle_number, 10.0f);
    ASSERT_FLOAT_EQ(simmulatedAnnealing->getProviderInfo().truck_capacity, 10.0f);

    ASSERT_FLOAT_EQ(simmulatedAnnealing->getCustomerNumber(), 4);
}