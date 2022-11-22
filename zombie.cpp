#include "zombie.h"



Zombie::Zombie(pair<int,int> p)
{
    speed = 1.f;
    health = 100.f;
    pos = p;
    update_time = 1.f;
    clock1.restart();
    clock2.restart();
    shape = new sf::RectangleShape(sf::Vector2f(65.f,65.f));
    texture = new sf::Texture();
    texture->loadFromFile("zombie7.png");
    shape->setTexture(texture);
    animation = new Animation(texture, sf::Vector2u(3,4), 0.5f);
}

Zombie::~Zombie()
{
    delete shape;
    delete texture;
    delete animation;
}


vector<pair<float,float>> Zombie::bfs(const vector<string>& mymap, pair<int,int>& zpos, pair<int,int>& ppos) 
{   
    list<pair<int,int>> q = {zpos};
    // vector<pair<int,int>> moves = { {1,0}, {1,1}, {-1,0}, {-1,1}, {0,1}, {-1,-1}, {0,-1}, {1,-1} };
    vector<pair<int,int>> moves = { {1,0}, {-1,0}, {0,1}, {0,-1}};
    map<pair<int,int>, pair<int,int>, Compare> parent;
    map<pair<int,int>, bool> seen;
    parent[zpos] = pair<int,int>(-1,-1);

    while (q.size() > 0) {
        pair<int,int> v = q.front();
        seen[v] = true;
        q.pop_front();
        if (v.first == ppos.first && v.second == ppos.second) {
            return shortest_path(parent, ppos, zpos);
        }
        for (pair<int,int> m : moves) {
            pair<int,int> u = {v.first + m.first, v.second + m.second};
            if (seen.find(u) == seen.end()) {
                parent[u] = v;
                if (mymap[u.first][u.second] != '#') {
                    q.push_back(pair<int,int>(u.first, u.second));
                }
            }
        }
    }
    return {};
}


vector<pair<float,float>> Zombie::shortest_path(map<pair<int,int>,pair<int,int>,Compare> &parents, pair<int,int> &player_pos, pair<int,int> &zombie_pos)
{
    vector<pair<float,float>> path;
    pair<int,int> v = player_pos;
    path.push_back(v);
    while (parents[v].first != zombie_pos.first || parents[v].second != zombie_pos.second) {
        v = parents[v];
        pair<float,float> n = {(float)v.first + 0.5, (float)v.second + 0.5};
        path.push_back(n);
    }
    return path;
}



void Zombie::update_zombie(vector<string>& mymap, pair<int,int>& ppos) 
{
    float dt = clock2.getElapsedTime().asSeconds();
    if (dt >= update_time) {
        pair<int,int> x = {(int)pos.first, (int)pos.second};
        path = bfs(mymap, x, ppos);
        clock2.restart();
    }
    
    if (path.size() > 0) {
        auto back = path.back();
        float dx = back.first - pos.first;
        float dy = back.second - pos.second;
        pair<float,float> direction = {dx/sqrt(pow(dx,2)+pow(dy,2)), dy/sqrt(pow(dx,2)+pow(dy,2))};
        auto prev_pos = pos;
        float d = 0.01;
        if (mymap[pos.first+d*direction.first][pos.second+d*direction.second] == '#') {
            pair<int,int> x = {(int)pos.first, (int)pos.second};
            path = bfs(mymap, x, ppos);
            clock2.restart();
            return;
        }
        pos.first += d * direction.first;
        pos.second += d * direction.second;
        if (mymap[pos.first][pos.second] == '-') {
            mymap[prev_pos.first][prev_pos.second] = '-';
        }
    }
}



void Zombie::update_animation()
{
    animation->update(0, clock1.getElapsedTime().asSeconds());
    clock1.restart();
    shape->setTextureRect(animation->uv_rect);
}


void Zombie::set_screen_pos(float zombiex, float zombiey, float zwidth)
{
    shape->setPosition(zombiex, zombiey);
    shape->setSize(sf::Vector2f(zwidth, zwidth));
}