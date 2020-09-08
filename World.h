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

        World(int size, int initial_predators, int initial_prey);

        vector<Agent, allocator<Agent>>::iterator getAgentAt(int x, int y);

        void placeIDAt(int id, int x, int y);

        void removeIDAt(int x, int y);

        void tick();

        void spawn_predator();

        void spawn_prey();
};

