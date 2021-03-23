#include <iostream>
#include <string>
#include "solution.hpp"
#include "simmulated_annealing.hpp"
#include <fstream>

int main(int argc, char * argv[]){

    SimmulatedAnnealing annealing;
    
    int e = annealing.parseDataFromFile("../tests/testDataFiles/m2kvrptw-0.txt");
    if(e == 1){
        CRPTW_Solution * solution = &annealing.findInitSolution("cprwt");

        for(int i = 0; i < solution->getNOfRoutes(); i++){
            for(int j = 0; j < solution->getRoute(i).getSizeOfroute(); j++){
                std::cout << solution->getRoute(i)[j].customer->id << " ";
            }
            std::cout<<std::endl;
        }        
        std::cout << solution->getNOfRoutes() << " " << solution->getTotalDistance() << " " << solution->getTotalTime() << std::endl;

    }

    return 0;
}