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
            // sf::Vertex edge[2] = {
            //     sf::Vertex(locations[ind1]),
            //     sf::Vertex(locations[ind2])
            // };
            // window->draw(edge, 2, sf::Lines);
            sf::Vector2f v = locations[ind1].position - locations[ind2].position;
            auto rv = sf::Vector2f(-v.y, v.x);
            sf::ConvexShape edge;
            edge.setPointCount(4);
            edge.setPoint(0, locations[ind1].position + rv * lineThickness * 0.5f / length(rv));
            edge.setPoint(1, locations[ind1].position - rv * lineThickness * 0.5f / length(rv));
            edge.setPoint(2, locations[ind2].position - rv * lineThickness * 0.5f / length(rv));
            edge.setPoint(3, locations[ind2].position + rv * lineThickness * 0.5f / length(rv));
            edge.setFillColor(COLORS[i % colorsNo]);

            sf::CircleShape edgeCompound(lineThickness * 0.5f);
            edgeCompound.setPosition(locations[ind1].position);
            edgeCompound.setFillColor(COLORS[i % colorsNo]);
            edgeCompound.setOrigin(sf::Vector2f(lineThickness, lineThickness) * 0.5f);
            
            this->window->draw(edgeCompound);
            this->window->draw(edge);
        }
    }
}

float Graph::length(sf::Vector2f v){

    return sqrt(v.x * v.x + v.y * v.y);
}