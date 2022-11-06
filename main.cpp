#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <math.h>
#include <iostream>

// g++ main.cpp -I/usr/local/Cellar/sfml/2.5.1_1/include/ -o main -L/usr/local/Cellar/sfml/2.5.1_1/lib/ -lsfml-graphics -lsfml-window -lsfml-system


using namespace std;

#define PI 3.14159

vector<string> mymap = {
    "##########################################",
    "#----------------------------------------#",
    "#----------------------------------------#",
    "#----------------#################-------#",
    "#----------------#################-------#",
    "#----------------############------------#",
    "#----------------############-----###----#",
    "#---------------------------------###----#",
    "#----------------------------------------#",
    "##########################################"
};

pair<float,float> player_pos = {5,5};
float player_angle = 0.2; // radians
float player_fov = PI / 3; // radians
int num_columns = 120;
float max_dist = 7;


class Screen
{
    float screen_width;
    float screen_height;
    sf::RenderWindow *window;
    float dangle;
    float col_width;

public:

    pair<float,bool> get_dist(float angle) {
        float x = player_pos.first;
        float y = player_pos.second;

        while (mymap[(int)x][(int)y] == '-') {
            x += 0.1 * cos(angle);
            y += 0.1 * sin(angle);
        }
        float dx = x - player_pos.first; 
        float dy = y - player_pos.second;
        float d = sqrt(pow(dx, 2) + pow(dy, 2));
        bool vertical;
        if (abs(round(x)-x) <= abs(round(y)-y)) {
            vertical = true;
            cout << "true" << endl;
        } else {
            vertical = false;
            cout << "false" << endl;
        }
        return pair<float,bool>(abs(d * cos(angle)), vertical);

    }


    void display_column(float dist, bool vertical, float angle) {
        int col_num = (int) ((angle / player_fov) * num_columns);
        float colx = (float) (screen_width * col_num / num_columns);
        float col_height = screen_height * (1 - dist / max_dist);
        // cout << "dist " << dist << endl; 
        if (col_height < 0) {
            col_height = 0;
        }
        float coly = (screen_height - col_height) / 2;

        sf::RectangleShape col = sf::RectangleShape(sf::Vector2f(col_width, col_height));

        if (vertical) {
            col.setFillColor(sf::Color(0,0,100));
        } else {
            col.setFillColor(sf::Color(0,0,200));
        }
        
        
        col.setPosition(colx, coly);
        window->draw(col);

    }


    Screen(sf::RenderWindow *w, float screenw, float screenh) {
        window = w;
        screen_width = screenw;
        screen_height = screenh;
        dangle = player_fov / num_columns;
        col_width = screen_width / num_columns;
    }

    void show_screen() {
        window->clear();
        float start_angle = player_angle - player_fov / 2;
        float angle = start_angle;
        for (int x=0; x < num_columns; x++) {
            pair<float,bool> dist_vertical = get_dist(angle);
            // float dist = get_dist(angle);
            display_column(dist_vertical.first, dist_vertical.second, angle - start_angle);
            angle += dangle;
        }
        window->display();
    }
};



class Player
{
    /* 
    get inputs from keyboard to update player_angle and position
    */

    void rotate(bool clockwise) {
        
        float dangle = 0.07/3;
        if (clockwise && player_angle < 2*PI - dangle) {
            player_angle += dangle;    
        } else if (clockwise && player_angle >= 2*PI - dangle) {
            player_angle = dangle;
        } else if (!clockwise && player_angle > dangle) {
            player_angle -= dangle;
        } else if (!clockwise && player_angle <= dangle) {
            player_angle = 2*PI - dangle;
        }
    }

    void move(bool forward) {
        
        float negative = -1.f;
        if (forward) {
            negative = 1.f;
        } 
        float d = 0.15/3;
        float dx = negative * d * cos(player_angle);
        float dy = negative * d * sin(player_angle);
        
        if (mymap[(int)(player_pos.first + dx)][(int)(player_pos.second) + dy] == '-') {
            player_pos.first += dx;
            player_pos.second += dy;
        } 
        cout << mymap[(int)(player_pos.first)][(int)(player_pos.second)] << endl;
    }



public:
    Player() {

    }

    void update_player() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            rotate(false);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            rotate(true);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            move(true);
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            move(false);
        }
    }
};



int main()
{
    
    float screenw = 1200;
    float screenh = 800;
    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(screenw, screenh), "FPS");
    window->setFramerateLimit(100);

    Screen screen(window, screenw, screenh);
    Player player;

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }

        player.update_player();
        screen.show_screen();
    }

    delete window;
}