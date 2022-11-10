#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <vector>
#include <list>
#include <map>
#include <math.h>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;


struct Compare
{
    bool operator() (const pair<int,int>& p1, const pair<int,int>& p2) const {
        return sqrt(pow(p1.first,2) + pow(p1.second, 2)) < sqrt(pow(p2.first,2) + pow(p2.second, 2));
    }
};


class Zombie
{
    
    float speed; 
    float health;
    sf::RectangleShape * shape;
    sf::Clock clock;
    float update_time;
    vector<pair<int,int> > bfs(const vector<string>& mymap, pair<int,int>& zpos, pair<int,int>& ppos);
    vector<pair<int,int> > shortest_path(map<pair<int,int>, pair<int,int>, Compare> &parents, pair<int,int> &player_pos, pair<int,int> &zombie_pos);
public:
    pair<float,float> pos;
    Zombie(pair<int,int> p);
    ~Zombie();
    void update_zombie(vector<string>& mymap, pair<int,int>& ppos);
};

#endif