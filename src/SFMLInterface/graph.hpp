#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "../main/solution.hpp"
#include <vector>
#include <utility>
#include <math.h>

const sf::Color COLORS[] = {
    sf::Color::White,
    sf::Color::Red,
    sf::Color::Cyan,
    sf::Color::Blue,
    sf::Color::Green,
    sf::Color::Yellow,
    sf::Color::Magenta
};

const int colorsNo = 7;

class Graph{

private:

    sf::VertexArray locations;

    float blobRadius = 1.f;
    float lineThickness = 1.f;

    sf::RenderWindow * window;
    sf::View * view;

    float length(sf::Vector2f v);

public:

    Graph(sf::RenderWindow & w, sf::View & v, std::vector<Customer> & customers, ProviderInfo & prov){
        
        this->locations.append(sf::Vertex(sf::Vector2f(prov.warehouse_x, prov.warehouse_y)));

        for(auto & customer : customers){
            this->locations.append(sf::Vertex(sf::Vector2f(customer.x, customer.y)));
        }

        this->window = &w;
        this->view = &v;
        auto bounds = this->locations.getBounds();

        view->setSize(sf::Vector2f(std::max(bounds.width, bounds.height), std::max(bounds.width, bounds.height)) * 1.1f);
        view->setCenter(sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f));
    }

    virtual ~Graph(){

    } 

    void drawGraph(CRPTW_Solution * solution);
};

#endif