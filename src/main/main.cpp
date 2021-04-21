#include <iostream>
#include <string>
#include "solution.hpp"
#include "simmulated_annealing.hpp"
#include <fstream>

int main(int argc, char * argv[]){

    srand(time(0));
    SimmulatedAnnealing annealing;
    
    int e = annealing.parseDataFromFile(SimmulatedAnnealing::getPathToWorkspaceFolder() + "tests/solomonInstances/solomon_100/RC105.txt");
    if(e == 1){
        // annealing.findInitSolution("insertion");
        annealing.runAlgorithm();

        std::cout <<"valid solution: " << CRPTW_Solution::isValid(*annealing.getSolution()) << std::endl;

        CRPTW_Solution * solution = annealing.getSolution();

        std::cout << *solution;
    }

    return 0;
}