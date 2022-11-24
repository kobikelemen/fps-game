#include "game.h"


Game::Game(vector<string>* m, vector<Zombie*>* z, vector<pair<int,int>> sp)
{
    zombies = z;
    map = m; 
    spawn_points = sp;
    round = 0;
}


Game::~Game()
{

}


void Game::spawn_zombie()
{
    if (zombies_spawned < zombies_to_spawn) {
        int sp = zombies_spawned % spawn_points.size();
        pair<int,int> x = spawn_points[sp];
        if ((*map)[x.first][x.second] == '-') {
            zombies_spawned ++;
            Zombie *z = new Zombie(x);
            zombies->push_back(z);
        }
    }
}


void Game::update()
{
    if (zombies->size() == 0) {
        round++;
        zombies_to_spawn = round * 2;
        zombies_spawned = 0;
    }
    spawn_zombie();
}