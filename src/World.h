#pragma once

#include <vector>
#include "Agent.h"
using namespace std;

class World {
    public:
        vector<vector<int>> grid;
        vector<Agent> agents;
        int num_predators;
        int num_prey;
        float predator_reproduction_chance;
        float prey_reproduction_chance;
        int predator_hunger_limit;

        World(int size, int initial_predators, int initial_prey,
              float predator_reproduction_chance, float prey_reproduction_chance,
              int predator_hunger_limit);

        vector<Agent, allocator<Agent>>::iterator getAgentAt(int x, int y);

        void placeIDAt(int id, int x, int y);

        void removeIDAt(int x, int y);

        void tick();

        int spawn_predator();

        int spawn_prey();

        void kill_predators();

};

