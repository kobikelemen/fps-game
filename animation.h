#ifndef ANIMATION_H
#define ANIMATION_H


#include <SFML/Graphics.hpp>


class Animation
{
    sf::Vector2u image_count;
    sf::Vector2u current_image;
    float total_time;
    

public:
    float switch_time;
    Animation(sf::Texture *texture, sf::Vector2u image_count, float switch_time);
    ~Animation();
    sf::IntRect uv_rect;
    void update(int row, float dt);
};

#endif