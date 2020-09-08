
#include <random>
#include <iostream>
#include "World.h"
#include "Rand.h"

#define WORLD_SIZE          100
#define INITIAL_PREDATORS   500
#define INITIAL_PREY        500
#define NUM_TICKS           10000

using namespace std;

int main() {
    World world{WORLD_SIZE, INITIAL_PREDATORS, INITIAL_PREY};
    // Generates some number of initial predators and puts them on the grid
    for (int i = 0; i < INITIAL_PREDATORS; i++) {
        // Generate some random ints
        int x_pos = generator() % WORLD_SIZE;
        int y_pos = generator() % WORLD_SIZE;
        // If there's already something there, generate a new random position
        if (world.grid[x_pos][y_pos] == -1) {
            world.agents.emplace_back(&world, true, x_pos, y_pos);
        }
    }

    // Generates some number of initial prey and puts them on the grid
    for (int i = 0; i < INITIAL_PREY; i++) {
        // Generate some random ints
        int x_pos = generator() % WORLD_SIZE;
        int y_pos = generator() % WORLD_SIZE;
        // If there's already something there, generate a new random position
        if (world.grid[x_pos][y_pos] == -1) {
            world.agents.emplace_back(&world, false, x_pos, y_pos);
        }
    }

    for (auto const& agent : world.agents) {
        world.placeIDAt(agent.id, agent.x_pos, agent.y_pos);
    }

    for (int i = 0; i < NUM_TICKS; i++) {
        if (i % 1000 == 0) {
            std::cout << "pred: " << world.num_predators << "\n";
            std::cout << "prey: " << world.num_prey << "\n";
        }

        world.tick();
    }

}