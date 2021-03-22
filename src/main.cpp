#include <iostream>
#include <string>
#include "simmulated_annealing.hpp"
#include <fstream>

int main(int argc, char * argv[]){

    SimmulatedAnnealing annealing;

    int e = annealing.parseDataFromFile("cvrptw4.txt");
    std::cout << e << std::endl;
    // std::fstream we;
    // we.open("asdcvrptw4.txt", std::ios::in);
    // if(we.is_open()){
    //     std::cout<< "successfully opened"<< std::endl;
    //     we.close();
    // }
    // else {

    //     std::cerr << "error " << errno;
    // }
}