#include <iostream>
#include <string>
#include "solution.hpp"
#include "simmulated_annealing.hpp"
#include <fstream>

int main(int argc, char * argv[]){

    srand(time(0));
    SimmulatedAnnealing annealing;
    
    int e = annealing.parseDataFromFile(SimmulatedAnnealing::getPathToWorkspaceFolder() + "tests/solomonInstances/solomon_100/RC105.txt");
    if(!e){

        annealing.runAlgorithm("insertion");
        
        std::cout << "Solution: "<< std::endl;
        std::cout << *annealing.getSolution();
    }

    return 0;
}