#include "graph.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

#include "../main/simmulated_annealing.hpp"
#include <pthread.h>
#include <stdlib.h>
#include <netdb.h>

bool thread_started = false;

void * calculationThread(void * args){
    thread_started = true;
    SimmulatedAnnealing * annealing = (SimmulatedAnnealing * )args;

    // annealing->findInitSolution("greedy", true);
    annealing->runAlgorithm("insertion", true);
}

int main(int argc, char * argv[])
{   

    srand(time(0));

    pthread_t calc_thread;
    SimmulatedAnnealing annealing;
    if(annealing.parseDataFromFile(SimmulatedAnnealing::getPathToWorkspaceFolder() + "tests/solomonInstances/solomon_50/R105.txt") == -1) return -1;


    // CRPTW_Solution * solution = annealing.getSolution();

    sf::RenderWindow window(sf::VideoMode(800, 800), "Demo");
    sf::View graphView, bestGraphView;
    graphView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
    bestGraphView.setViewport(sf::FloatRect(0.7f, 0.7f, 0.3f, 0.3f));

    Graph graph(window, graphView, annealing.getCustomers(), annealing.getProviderInfo());
    Graph bestGraph(window, bestGraphView, annealing.getCustomers(), annealing.getProviderInfo());

    sf::Font font;
    if(!font.loadFromFile(SimmulatedAnnealing::getPathToWorkspaceFolder() + "src/SFMLInterface/Open_Sans/OpenSans-Bold.ttf")){
        std::cout << "cannot load font" << std::endl;
    }
    // sf::Text text;
    // text.setFont(font);
    // text.setString("Best Solution");
    // text.setCharacterSize(60);
    // text.setColor(sf::Color::White);
    // window.draw(text);

    sf::Vector2f lastMousePos = window.mapPixelToCoords(sf::Mouse::getPosition());
    bool canMoveMap = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if(event.type == sf::Event::KeyPressed){
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                        
                    int status = pthread_create(&calc_thread, NULL, calculationThread, (void *)&annealing);

                    if(status){
                        fprintf(stderr, "creating server thread: %s", gai_strerror(status));
                        window.close();
                        return -1;
                    }
                }
            }
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
            if(event.type == sf::Event::Resized){

                float new_width, new_height;
                new_width = event.size.width;
                new_height = event.size.height;

                float ratio = new_height / new_width;

                graphView.setSize(sf::Vector2f(graphView.getSize().x, graphView.getSize().x * ratio));
            }
        }

        window.clear(sf::Color::Black);
        
        pthread_mutex_lock(&annealing.getOperationMutex());
        graph.drawGraph(annealing.getSolution());
        bestGraph.drawGraph(annealing.getBestSolution());
        pthread_mutex_unlock(&annealing.getOperationMutex());

        window.setFramerateLimit(60);
        window.display();
    }
    if(thread_started)
        pthread_join(calc_thread, NULL);

    return 0;
}