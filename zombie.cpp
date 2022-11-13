#include "zombie.h"



Zombie::Zombie(pair<int,int> p)
{
    speed = 1.f;
    health = 100.f;
    pos = p;
    update_time = 1.f;
    clock.restart();
    shape = new sf::RectangleShape(sf::Vector2f(100.f,150.f));
    texture = new sf::Texture();
    texture->loadFromFile("zombie7.png");
    shape->setTexture(texture);
    animation = new Animation(texture, sf::Vector2u(3,4), 0.5f);
    sf::Clock clock;
}

Zombie::~Zombie()
{
    delete shape;
    delete texture;
}


vector<pair<int,int>> Zombie::bfs(const vector<string>& mymap, pair<int,int>& zpos, pair<int,int>& ppos) 
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
            // for (auto p : parent) {
            //     cout << p.first.first << " " << p.first.second << " --> " << p.second.first << " " << p.second.second << endl;
            // }
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


vector<pair<int,int>> Zombie::shortest_path(map<pair<int,int>,pair<int,int>,Compare> &parents, pair<int,int> &player_pos, pair<int,int> &zombie_pos)
{
    vector<pair<int,int>> path;
    pair<int,int> v = player_pos;
    path.push_back(v);
    while (parents[v].first != zombie_pos.first || parents[v].second != zombie_pos.second) {
        v = parents[v];
        path.push_back(v);
    }
    return path;
}


void Zombie::update_zombie(vector<string>& mymap, pair<int,int>& ppos) 
{

    if (clock.getElapsedTime().asSeconds() >= update_time) {
        pair<int,int> x = {(int)pos.first, (int)pos.second};
        vector<pair<int,int>> path = bfs(mymap, x, ppos);
        auto back = path.back();
        if (mymap[back.first][back.second] == '-') {
            pos = back;
        }
        clock.restart();
    }
}



void Zombie::update_animation()
{
    animation->update(0, dt);
    shape->setTextureRect(animation->uv_rect);
}


void Zombie::set_screen_pos(float zombiex, float zombiey, float zwidth)
{
    shape->setPosition(zombiex, zombiey);
    shape->setSize(sf::Vector2f(zwidth, zwidth));

}