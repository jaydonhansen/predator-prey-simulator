#pragma once

class World;

class Agent {
    public:
    Agent(World *world, bool predator, int x_pos, int y_pos);

    static int id_;
        int id;
        int x_pos;
        int y_pos;
        bool predator;
        int hunger;
        World* world;

        int eat(int x, int y);

        void move(int x, int y);
};

