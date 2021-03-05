#include <iostream>
#include <string>

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"

void draw_current_solution(sf::RenderWindow & window, sf::View & view){

}

void draw_solution_info(sf::RenderWindow & window, sf::View & view){

}

int main(int argc, char * argv[]){

    sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(600,800), "simmulated annealing");
    sf::View view;
    view.setViewport(sf::FloatRect(0.0f, 0.0f, 1.0f, 1.0f));
    window.setView(view);

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        window.setFramerateLimit(60);
        window.display();
    }

    return 1;
}