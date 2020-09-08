#pragma once

#include <vector>
#include "Agent.h"
using namespace std;

class World {
    public:
        vector<vector<int>> grid;
        vector<Agent> agents;

        World(int size);

        vector<Agent, allocator<Agent>>::iterator getAgentAt(int x, int y);

        void placeIDAt(int id, int x, int y);

        void removeIDAt(int x, int y);

        void tick();
};

