#include "graph.hpp"


void Graph::drawGraph(CRPTW_Solution * solution){

    window->setView(*view);
    sf::CircleShape CustomerBlob(blobRadius);
    CustomerBlob.setFillColor(sf::Color::Black);
    CustomerBlob.setOutlineColor(sf::Color::White);
    CustomerBlob.setOutlineThickness(0.1f);
    CustomerBlob.setOrigin(sf::Vector2f(blobRadius, blobRadius));

    for(int i = 1; i < locations.getVertexCount(); i++){

        CustomerBlob.setPosition(locations[i].position);
        window->draw(CustomerBlob);
    }

    CustomerBlob.setPosition(locations[0].position);
    CustomerBlob.setFillColor(sf::Color::Red);
    window->draw(CustomerBlob);

    for(int i = 0; i < solution->getNOfRoutes(); i++){

        for(int j = 0; j < solution->getRoute(i).getSizeOfroute() - 1; j++){

            int ind1, ind2;
            ind1 = solution->getRoute(i)[j].customer->id;
            ind2 = solution->getRoute(i)[j + 1].customer->id;
            sf::Vertex edge[2] = {
                sf::Vertex(locations[ind1]),
                sf::Vertex(locations[ind2])
            };
            window->draw(edge, 2, sf::Lines);
        }
    }
}