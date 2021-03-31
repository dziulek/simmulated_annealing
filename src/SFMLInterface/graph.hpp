#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "../main/solution.hpp"
#include <vector>
#include <utility>



class Graph{

private:

    sf::VertexArray locations;
    sf::Vertex magazine;

    float blobRadius = 1.f;

public:

    Graph(std::vector<Customer> & customers, ProviderInfo & prov){

        for(auto & customer : customers){
            this->locations.append(sf::Vertex(sf::Vector2f(customer.x, customer.y)));
        }

        this->magazine = sf::Vertex(sf::Vector2f(prov.warehouse_x, prov.warehouse_y));
    }

    virtual ~Graph(){

    } 

    void drawGraph(sf::RenderWindow & target, sf::View & view, CRPTW_Solution * solution);
};

#endif