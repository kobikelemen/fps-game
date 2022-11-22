
#ifndef GUN_H
#define GUN_H

#include "animation.h"

class Gun
{   
    bool currently_shooting;
    int img_num;
    sf::Texture * texture;
    Animation * animation;
    sf::Clock clock;
public:
    sf::RectangleShape * shape;
    Gun(float screenw, float screenh);
    ~Gun();
    void update_animation(bool shoot);
};


#endif