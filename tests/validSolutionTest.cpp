#include "gtest/gtest.h"

#include "../src/main/simmulated_annealing.hpp"
#include "../src/main/solution.hpp"
#include <string>

TEST(valid_solution, solomon_instances_50){

    std::string tempArray[] = {"RC", "R", "C"};
    int fileToTestNo = 10;
    std::string fileName;

    SimmulatedAnnealing annealing;

    for(int i = 0; i < fileToTestNo; i++){

        fileName = tempArray[rand()%3] + std::to_string(rand()%2 + 1) + "0" + std::to_string(rand()%10) + ".txt";


        //to do clear annealing
        int b = annealing.parseDataFromFile(SimmulatedAnnealing::getPathToWorkspaceFolder() + "tests/solomonInstances/solomon_50/" + fileName);
        std::cerr << fileName << std::endl;
        if(b == -1)
            ASSERT_EQ(true, false);
        annealing.runAlgorithm();
        
        ASSERT_EQ(CRPTW_Solution::isValid(*annealing.getSolution()), true);
    }
    
}