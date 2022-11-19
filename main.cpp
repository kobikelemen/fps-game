#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <math.h>
#include <iostream>

#include "zombie.h"


using namespace std;

#define PI 3.14159


vector<string> mymap = {
    "##########################################",
    "#----------------------------------------#",
    "#----------------------------------------#",
    "#-------------############---------------#",
    "#-------------############---------------#",
    "#-------------############---------------#",
    "#-------------############--------###----#",
    "#---------------------------------###----#",
    "#----------------------------------------#",
    "##########################################"
};



pair<float,float> player_pos = {6,5};
float player_angle = 0; // radians
float player_fov = PI / 4; // radians
// int num_columns = 120;
int num_columns = 1200;
float max_dist = 7;



void print_map()
{
    vector<pair<int,int>> m = { {1,1}, {0,1}, {-1,1}, {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {0,1} };
    for (auto x : m) {
        if (mymap[(int)player_pos.first+x.first][(int)player_pos.second+x.second] == 'X') {
            mymap[(int)player_pos.first+x.first][(int)player_pos.second+x.second] = '-';
        }
    }
    mymap[(int)player_pos.first][(int)player_pos.second] = 'X';
    cout << endl << endl << endl;
    for (string s : mymap) {
        cout << s << endl;
    }
 }


void update_zombies(vector<Zombie*>& zombies) 
{
    for (Zombie *z : zombies) {
        mymap[(int)z->pos.first][(int)z->pos.second] = '-';
        pair<int,int> p = {(int)player_pos.first, (int)player_pos.second};
        z->update_zombie(mymap, p);
        mymap[(int)z->pos.first][(int)z->pos.second] = 'Z';
    }
}



class Screen
{
    float screen_width;
    float screen_height;
    sf::RenderWindow *window;
    float dangle;
    float col_width;

public:


    pair<float,bool> get_dist(float angle, char c) {
        float x = player_pos.first;
        float y = player_pos.second;
        while (mymap[(int)x][(int)y] != c) {
            x += 0.1 * cos(angle);
            y += 0.1 * sin(angle);
            if (int(x) >= mymap.size() || int(x) < 0 || int(y) >= mymap[0].size() || int(y) < 0) {
                return {-1.f,false};
            }
        }
        float dx = x - player_pos.first; 
        float dy = y - player_pos.second;
        float d = sqrt(pow(dx, 2) + pow(dy, 2));
        bool vertical;
        if (abs(round(x)-x) <= abs(round(y)-y)) {
            vertical = true;
        } else {
            vertical = false;
        }
        return pair<float,bool>(d, vertical);

    }


    void display_column(float dist, bool vertical, float angle, float height_reduction, int colour) {
        int col_num = (int) ((angle / player_fov) * num_columns);
        float colx = (float) (screen_width * col_num / num_columns);
        float ceiling = screen_height/2 - (screen_height - height_reduction)/dist;
        float col_height = screen_height - 2*ceiling;
        if (col_height < 0) {
            col_height = 0;
        }
        float coly = (screen_height - col_height) / 2;
        sf::RectangleShape col = sf::RectangleShape(sf::Vector2f(col_width, col_height));
        if (colour == 1) {
            if (vertical) {
                col.setFillColor(sf::Color(0,0,100));
            } else {
                col.setFillColor(sf::Color(0,0,200));
            }
        } else if (colour == 2) {
            col.setFillColor(sf::Color::Red);
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

    void show_screen(vector<Zombie*>& zombies) {
        window->clear();
        float start_angle = player_angle - player_fov / 2;
        float angle = start_angle;
        for (int x=0; x < num_columns; x++) {
            pair<float,bool> dist_vertical = get_dist(angle, '#');
            display_column(dist_vertical.first, dist_vertical.second, angle - start_angle, 0, 1);
            angle += dangle;
        }
        show_zombies(zombies);
        window->display();
    }


    float player_to_zombie_angle(pair<int,int> zpos, pair<int,int> ppos) {
        /* player line of sight angle to zombie */
        float denominator = zpos.first - ppos.first;
        if (denominator == 0) {
            float res;
            if (ppos.second > zpos.second) {
                return PI/2;
            } else {
                return 3*PI/2;
            }   
        }
        if (ppos.first > zpos.first) {
            return atan((ppos.second - zpos.second) / (denominator)) + PI;
        }
        if (ppos.first < zpos.first && ppos.second < zpos.second) {
            return atan((ppos.second - zpos.second) / (denominator)) + 2*PI;
        }
        return atan((ppos.second - zpos.second) / (denominator));
        
    }

    float zombie_width(pair<float,float> zpos, pair<float,float> ppos) {
        /* given distance between zombie and player, returns zombie x size */
        float dist = sqrt(pow(zpos.first - ppos.first, 2) + pow(zpos.second - ppos.second, 2));
        float ceiling = screen_height/2 - (screen_height)/dist;
        float height = screen_height - 2*ceiling;
        return height; // width = height here
    }

    void show_a_zombie(Zombie* z) 
    {
        float rel_angle = player_to_zombie_angle(z->pos, player_pos);
        float x = -(abs(rel_angle - (PI-player_angle)) - PI);
        if (abs(x) < player_fov / 2) {
            float midx = screen_width * (player_fov/2 - x) / player_fov;
            float zwidth = zombie_width(z->pos, player_pos);
            float zombiex = midx - zwidth/2;
            float zombiey = screen_height/2 - zwidth/4;
            if (midx - zwidth/2 > 0 && midx + zwidth/2 < screen_width) {
                z->set_screen_pos(zombiex, zombiey, zwidth);
                z->update_animation();
                window->draw(*z->shape);
            } 
        }
    }

    

    bool wall_blocking(Zombie* z)
    {
        float angle = atan((0.1 + z->pos.second - player_pos.second) / (0.1 + z->pos.first - player_pos.first));
        if (player_pos.first >= z->pos.first) {
            angle += PI;
        }
        float x = player_pos.first;
        float y = player_pos.second;
        vector<pair<int,int>> poses = {{(int)x, (int)y}};
        while (mymap[(int)x][(int)y] != 'Z') {
            if (poses.back().first != (int)x || poses.back().second != (int)y) {
                poses.push_back({(int)x, (int)y});
            }
            x += 0.1 * cos(angle);
            y += 0.1 * sin(angle);
            if (mymap[(int)x][(int)y] == '#') {
                return true;
            }
        }
        return false;
    }


    void show_zombies(vector<Zombie*>& zombies)
    {
        for (Zombie* z : zombies) {
            if (!wall_blocking(z)) {
                show_a_zombie(z);
            }
        }
    }
};



class Player
{
    /* 
    gets inputs from keyboard & mouse to update player_angle and position
    */

   sf::Vector2i mousepos;
   float screenw;
   float screenh;

    void rotate() {
        sf::Vector2i newmousepos = sf::Mouse::getPosition();
        if (newmousepos.x < 0) {
            newmousepos = sf::Vector2i(0, newmousepos.y);
        } else if (newmousepos.x > screenw) {
            newmousepos = sf::Vector2i(screenw, newmousepos.y);
        }
        float mouse_sensitivity = 3.f;
        float dangle = mouse_sensitivity * player_fov * (newmousepos.x - mousepos.x) / screenw;
        player_angle += dangle;
        mousepos = sf::Vector2i(screenw/2, screenh/2);
        sf::Mouse::setPosition(mousepos);
        player_angle = fmod(player_angle, 2*PI); //
    }

    void move(bool forward, bool left, bool vertical, bool horizontal) {
        
        float negative = -1.f;
        if (forward || left) {
            negative = 1.f;
        } 
        float d = 0.15/3;
        float dx, dy;
        if (vertical) { 
            dx = negative * d * cos(player_angle);
            dy = negative * d * sin(player_angle);
        } else if (horizontal) {
            float horiz_sensitivity = 0.5;
            dy = negative * horiz_sensitivity * d * cos(PI-player_angle);
            dx = negative * horiz_sensitivity * d * sin(PI-player_angle);
        }
        
        if (
            mymap[(int)(player_pos.first + dx)][(int)(player_pos.second + dy)] == '-' 
            || mymap[(int)(player_pos.first + dx)][(int)(player_pos.second + dy)] == 'X'
            ) {
            player_pos.first += dx;
            player_pos.second += dy;
        }
    }

public:

    Player(float screenw, float screenh) {
        this->screenw = screenw;
        this->screenh = screenh;
        mousepos = sf::Vector2i(screenw, screenh);
    }

    void update_player() {
        rotate();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
            move(false, true, false, true);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
            move(false, false, false, true);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) 
            move(true, false, true, false);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
            move(false, false, true, false);
    }
};



int main()
{
    
    float screenw = 1200;
    float screenh = 800;
    sf::RenderWindow *window = new sf::RenderWindow(sf::VideoMode(screenw, screenh), "FPS");
    window->setFramerateLimit(100);
    Screen screen(window, screenw, screenh);
    Player player(screenw, screenh);
    vector<Zombie*> zombies;
    Zombie *z1 = new Zombie({5,5});
    zombies.push_back(z1);


    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window->close();
        }
        print_map();
        update_zombies(zombies);
        player.update_player();
        screen.show_screen(zombies);
        
    }
    delete window;
}