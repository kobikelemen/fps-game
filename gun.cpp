#include "gun.h"


Gun::Gun(float screenw, float screenh)
{
    currently_shooting = false;
    img_num = 0;
    clock.restart();
    sf::Vector2f shape_dim = sf::Vector2f(450.f,450.f);
    shape = new sf::RectangleShape(shape_dim);
    shape->setPosition(screenw/2 - shape_dim.x/2, screenh - shape_dim.y);
    texture = new sf::Texture();
    texture->loadFromFile("gun.png");
    shape->setTexture(texture);
    animation = new Animation(texture, sf::Vector2u(6,1), 0.085f);
}


Gun::~Gun()
{
    delete texture;
    delete shape;
    delete animation;
}


void Gun::update_animation(bool shoot)
{
    if (shoot && !currently_shooting) {
        currently_shooting = true;
    }
    if (currently_shooting) {
        float t = clock.getElapsedTime().asSeconds();
        if (t > animation->switch_time) {
            img_num ++;
            clock.restart();
            animation->update(0, t);
        }
        shape->setTextureRect(animation->uv_rect);
        if (img_num == 6) {
            img_num = 0;
            currently_shooting = false;
        }
    }
}