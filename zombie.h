#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "animation.h"

using namespace std;




struct Compare
{
    bool operator() (const pair<int,int>& p1, const pair<int,int>& p2) const {
        int mapsizex = 42;
        int one = p1.second + p1.first * mapsizex;
        int two = p2.second + p2.first * mapsizex;
        return one < two;
    }
};


class Zombie
{
    
    float speed; 
    float health;
    sf::Texture * texture;
    Animation * animation;
    sf::Clock clock;
    float dt;
    float update_time;
    vector<pair<float,float>> path;
    vector<pair<float,float> > bfs(const vector<string>& mymap, pair<int,int>& zpos, pair<int,int>& ppos);
    vector<pair<float,float> > shortest_path(map<pair<int,int>, pair<int,int>, Compare> &parents, pair<int,int> &player_pos, pair<int,int> &zombie_pos);
public:
    pair<float,float> pos;
    sf::RectangleShape * shape;
    Zombie(pair<int,int> p);
    ~Zombie();
    void update_zombie(vector<string>& mymap, pair<int,int>& ppos);
    void set_screen_pos(float zombiex, float zombiey, float zwidth);
    void update_animation();
};

#endif