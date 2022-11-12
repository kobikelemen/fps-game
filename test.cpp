#include <SFML/Graphics.hpp>
#include <iostream>
#include "animation.h"

// 192 x 256

int main()
{
    sf::RenderWindow window(sf::VideoMode(1200, 600), "SFML works!");
    sf::RectangleShape player(sf::Vector2f(100.f,150.f));
    player.setPosition(206.f,206.f);
    sf::Texture texture;
    texture.loadFromFile("zombie7.png");
    player.setTexture(&texture);

    Animation animation(&texture, sf::Vector2u(3,4), 0.5f);

    
    sf::Clock clock;
    float dt;
    
    while (window.isOpen())
    {
        
        dt = clock.restart().asSeconds();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        animation.update(0, dt);
        player.setTextureRect(animation.uv_rect);


        window.clear();
        window.draw(player);
        window.display();
    }

    return 0;
}