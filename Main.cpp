
#include <random>
#include <iostream>
#include "World.h"
#include "Rand.h"

using namespace std;

int main() {


    World world{10};
    for (int i = 0; i < 10; i++) {
        // Generate some random ints
        int x_pos = generator() % 10;
        int y_pos = generator() % 10;
        // If there's already something there, generate a new random position
        if (world.grid[x_pos][y_pos] == -1) {
            world.agents.emplace_back(&world, true, x_pos, y_pos);
        }
    }

    for (int i = 0; i < 10; i++) {
        // Generate some random ints
        int x_pos = generator() % 10;
        int y_pos = generator() % 10;
        // If there's already something there, generate a new random position
        if (world.grid[x_pos][y_pos] == -1) {
            world.agents.emplace_back(&world, false, x_pos, y_pos);
        }
    }

    for (auto const& agent : world.agents) {
        world.placeIDAt(agent.id, agent.x_pos, agent.y_pos);
    }
    world.tick();
    std::cout << world.agents.size();
}