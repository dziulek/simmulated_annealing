#include "gtest/gtest.h"
#include "../src/main/simmulated_annealing.hpp"
#include <iostream>
#include <cmath>
#include <unistd.h>

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

    char directory[FILENAME_MAX];
    getcwd(directory, FILENAME_MAX);

    b = simmulatedAnnealing->parseDataFromFile("../tests/testDataFiles/cvrptw4.txt");
    // std::cerr << std::string(directory) << std::endl;

    ASSERT_EQ(b, 1);
    ASSERT_FLOAT_EQ(simmulatedAnnealing->getProviderInfo().vehicle_number, 10.0f);
    ASSERT_FLOAT_EQ(simmulatedAnnealing->getProviderInfo().truck_capacity, 10.0f);

    ASSERT_FLOAT_EQ(simmulatedAnnealing->getCustomerNumber(), 3);
    ASSERT_FLOAT_EQ(simmulatedAnnealing->getProviderInfo().due_date, 100.0f);
}