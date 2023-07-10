#include <math.h>
#include <stdio.h>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

using namespace std;

int main()
{
    sf::Window window;
    sf::Event event;
    window.create(sf::VideoMode(800, 600), "My window");
    //window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed)
        
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                cout << "left" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                cout << "right" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                cout << "forward" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                cout << "back" << endl;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                cout << "close" << endl;
                window.close();
            }
        
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    cout << "xD";
    return 0;
}