#include "zombie.h"



Zombie::Zombie(pair<int,int> p)
{
    speed = 1.f;
    health = 100.f;
    pos = p;
    shape = new sf::RectangleShape(sf::Vector2f(50,100));
    shape->setFillColor(sf::Color::Red);
}

Zombie::~Zombie()
{
    delete shape;
}


vector<pair<int,int>> Zombie::bfs(const vector<vector<char>>& mymap, const pair<int,int>& zpos, pair<int,int>& ppos) 
{   
    list<pair<int,int>> q = {zpos};
    vector<pair<int,int>> moves = { {1,0}, {-1,0}, {0,1}, {0,-1} };
    map<pair<int,int>, pair<int,int>, Compare> parent;
    parent[zpos] = pair<int,int>(-1,-1);

    while (q.size() > 0) {
        pair<int,int> v = q.front();
        q.pop_front();
        if (v.first == ppos.first && v.second == ppos.second) {
            return shortest_path(parent, ppos);
        }
        for (pair<int,int> m : moves) {
            pair<int,int> u = {v.first + m.first, v.second + m.second};
            parent[u] = v;
            if (mymap[u.first][u.second] == '-') {
                q.push_back(pair<int,int>(u.first, u.second));
            }
        }
    }
    return {};
}


vector<pair<int,int>> Zombie::shortest_path(map<pair<int,int>,pair<int,int>,Compare> &parents, pair<int,int> &player_pos)
{
    vector<pair<int,int>> path;
    pair<int,int> v = player_pos;
    path.push_back(v);
    while (parents[v].first != -1 && parents[v].second != -1) {
        v = parents[v];
        path.push_back(v);
    }
    return path;
}


void Zombie::update_zombie(vector<vector<char>>& mymap, pair<int,int>& ppos) 
{
    vector<pair<int,int>> path = bfs(mymap, pos, ppos);
    pos = path.back();
}

void Zombie::draw(sf::RenderWindow *window, pair<int,int> ppos) 
{
    float dist = sqrt(pow(ppos.first - pos.first, 2) + pow(ppos.second - pos.second,2));
    // TODO
    float height = screen_
    window->draw(*shape);
}