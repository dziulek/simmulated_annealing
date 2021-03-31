#include <iostream>
#include <string>
#include "solution.hpp"
#include "simmulated_annealing.hpp"
#include <fstream>

int main(int argc, char * argv[]){

    srand(time(0));
    SimmulatedAnnealing annealing;
    
    int e = annealing.parseDataFromFile("/home/czewian/Dokumenty/simmulated_annealing/tests/testDataFiles/m2kvrptw-0.txt");
    if(e == 1){
        annealing.runAlgorithm();
        CRPTW_Solution * solution = annealing.getSolution();

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