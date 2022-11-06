#include <vector>
#include <list>
#include <map>
#include <math.h>

#ifndef ZOMBIE_H
#define ZOMBIE_H


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
    vector<pair<int,int>> bfs(const vector<vector<char>>& mymap, const pair<int,int>& zpos, pair<int,int>& ppos);
    vector<pair<int,int>> shortest_path(map<pair<int,int>, pair<int,int>, Compare> &parents, pair<int,int> &player_pos);
public:
    Zombie();
    void update_zombie();

};

#endif