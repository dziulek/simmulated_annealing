#include "graph.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "../main/simmulated_annealing.hpp"

int main()
{   
    SimmulatedAnnealing annealing;
    annealing.parseDataFromFile("/home/czewian/Dokumenty/simmulated_annealing/tests/solomonInstances/solomon_50/C101.txt");

    annealing.findInitSolution("greedy");

    CRPTW_Solution * solution = annealing.getSolution();



    sf::RenderWindow window(sf::VideoMode(800, 800), "My window");
    sf::View graphView;
    graphView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    window.setView(graphView);

    Graph graph(window, graphView, annealing.getCustomers(), annealing.getProviderInfo());

    sf::Vector2f lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition());
    bool canMoveMap = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::MouseWheelScrolled){

                if(event.mouseWheelScroll.delta < 0){
                    
                    graphView.zoom(0.9);
                }
                else graphView.zoom(1.1);
            }
            if(event.type == sf::Event::MouseButtonPressed){
                
                canMoveMap = true;
                lastMousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            }
            if(event.type == sf::Event::MouseButtonReleased){

                canMoveMap = false;
            }
            if(event.type == sf::Event::MouseMoved){

                if(canMoveMap){
                    sf::Vector2f v = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y)) - lastMousePos;

                    graphView.setCenter(graphView.getCenter() - v);
                    lastMousePos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                }
            }
        }

        window.clear(sf::Color::Black);
        
        graph.drawGraph(solution);
        window.display();
    }

    return 0;
}