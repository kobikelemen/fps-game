#include <SFML/Graphics.hpp>
#include <iostream>
#include "animation.h"
#include "gun.h"
// 192 x 256

int main()
{
    float screenw = 1200.f;
    float screenh = 600.f;
    sf::RenderWindow window(sf::VideoMode(screenw, screenh), "SFML works!");
    Gun gun(screenw, screenh);
    
    while (window.isOpen())
    {        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        gun.update_animation(true);
        window.clear();
        window.draw(*gun.shape);
        window.display();
    }

    return 0;
}