#include "graph.hpp"


void Graph::drawGraph(sf::RenderWindow & target, sf::View & view, CRPTW_Solution * solution){

    target.setView(view);
    sf::CircleShape CustomerBlob(blobRadius);
    CustomerBlob.setFillColor(sf::Color::Black);
    CustomerBlob.setOutlineColor(sf::Color::White);
    CustomerBlob.setOutlineThickness(0.1f);
    CustomerBlob.setOrigin(sf::Vector2f(blobRadius, blobRadius));

    for(int i = 0; i < locations.getVertexCount(); i++){

        CustomerBlob.setPosition(locations[i].position);
        target.draw(CustomerBlob);
    }

    for(int i = 0; i < solution->getNOfRoutes(); i++){

        for(int j = 0; j < solution->getRoute(i).getSizeOfroute() - 1; j++){

            int ind1, ind2;
            ind1 = solution->getRoute(i)[j].customer->id;
            ind2 = solution->getRoute(i)[j + 1].customer->id;
            sf::Vertex edge[2] = {
                sf::Vertex(locations[ind1]),
                sf::Vertex(locations[ind2])
            };
            target.draw(edge, 2, sf::Lines);
        }
    }
}