#ifndef GAME_H
#define GAME_H

#include "zombie.h"

using namespace std;

class Game
{
    int round;
    int zombies_to_spawn;
    int zombies_spawned;
    vector<Zombie*>* zombies;
    vector<string>* map;
    vector<pair<int,int>> spawn_points;
    void spawn_zombie();

public:

    Game(vector<string>* m, vector<Zombie*>* z, vector<pair<int,int>> sp);
    ~Game();
    void update();
};

#endif