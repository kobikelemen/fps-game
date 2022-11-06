#include "zombie.h"



Zombie::Zombie()
{
    speed = 1.f;
    health = 100.f;
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
            // TODO -- build shortest path from parent map and return
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
}


vector<pair<int,int>> shortest_path(map<pair<int,int>, pair<int,int>, Compare> &parents, pair<int,int> &player_pos)
{
    vector<pair<int,int>> path;

}

