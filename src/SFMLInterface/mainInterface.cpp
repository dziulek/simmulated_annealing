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

    sf::View statsView;
    statsView.setViewport(sf::FloatRect(0.7f, 0.1f, 0.3f, 0.5f));

    Graph graph(window, graphView, annealing.getCustomers(), annealing.getProviderInfo());
    Graph bestGraph(window, bestGraphView, annealing.getCustomers(), annealing.getProviderInfo());

    sf::Font font;
    if(!font.loadFromFile(SimmulatedAnnealing::getPathToWorkspaceFolder() + "src/SFMLInterface/Open_Sans/OpenSans-Bold.ttf")){
        std::cout << "cannot load font" << std::endl;
    }
    sf::Text text;
    text.setFont(font);
    text.setString("Best Solution");
    text.setCharacterSize(60);
    text.setColor(sf::Color::White);
    float text_width = text.getLocalBounds().width, text_height = text.getLocalBounds().height;
    text.setOrigin(sf::Vector2f(text_width / 2, text_height / 2));
    text.setPosition(sf::Vector2f(statsView.getSize().x / 2, text_height / 2));

    sf::Text text_curr;
    text_curr.setFont(font);
    text_curr.setString("Current Solution");
    text_curr.setCharacterSize(60);
    text_curr.setColor(sf::Color::White);
    float width_text_curr = text_curr.getLocalBounds().width, height_text_curr = text_curr.getLocalBounds().height;
    text_curr.setOrigin(sf::Vector2f(width_text_curr / 2, height_text_curr / 2));
    text_curr.setPosition(sf::Vector2f(statsView.getSize().x / 2, height_text_curr / 2 + 6.1 * text_height));

    sf::Text text_init;
    text_init.setFont(font);
    text_init.setString("Initial Solution");
    text_init.setCharacterSize(60);
    text_init.setColor(sf::Color::White);
    float width_text_init = text_init.getLocalBounds().width, height_text_init = text_init.getLocalBounds().height;
    text_init.setOrigin(sf::Vector2f(width_text_init / 2, height_text_init / 2));
    text_init.setPosition(sf::Vector2f(statsView.getSize().x / 2, height_text_init / 2 + 12.2 * text_height));

    sf::Text text_cost;
    text_cost.setFont(font);
    text_cost.setString("0");
    text_cost.setCharacterSize(70);
    text_cost.setColor(sf::Color::White);
    float width_text_cost = text_cost.getLocalBounds().width, height_text_cost = text_cost.getLocalBounds().height;
    text_cost.setOrigin(sf::Vector2f(width_text_cost / 2, height_text_cost / 2));
    text_cost.setPosition(sf::Vector2f(statsView.getSize().x / 2, height_text_cost / 2 + 12.2 * text_height));

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

        // window.setView(statsView);
        // window.draw(text);
        // text_cost.setPosition(sf::Vector2f(statsView.getSize().x / 2, 3.05 * text_height));
        // text_cost.setString(std::to_string((int)annealing.getBestSolution()->getTotalTime()));
        // window.draw(text_cost);
        // window.draw(text_curr);
        // text_cost.setPosition(sf::Vector2f(statsView.getSize().x / 2, 9.05 * text_height));
        // text_cost.setString(std::to_string((int)annealing.getSolution()->getTotalTime()));
        // window.draw(text_cost);
        // window.draw(text_init);
        // text_cost.setPosition(sf::Vector2f(statsView.getSize().x / 2, 15.05 * text_height));
        // if(annealing.status == INIT_SOLUTION)
        //     text_cost.setString(std::to_string((int)annealing.getSolution()->getTotalTime()));
        // window.draw(text_cost);

        
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