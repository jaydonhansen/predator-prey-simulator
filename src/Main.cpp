#include <fstream>
#include <iostream>
#include "World.h"
#include "Rand.h"

#define WORLD_SIZE                      1000    // Size of the world
#define INITIAL_PREDATORS               4000    // Predator n(0)
#define INITIAL_PREY                    20000    // Prey n(0)
#define PREDATOR_REPRODUCTION_CHANCE    0.1   // Chance of predators reproducing
#define PREY_REPRODUCTION_CHANCE        1.0   // Chance of prey reproducing
#define PREDATOR_HUNGER_LIMIT           3     // How hungry a predator can get before dying
#define NUM_TICKS                       100    // Number of times to iterate

using namespace std;
int main() {
    World world{WORLD_SIZE, INITIAL_PREDATORS, INITIAL_PREY,
                PREDATOR_REPRODUCTION_CHANCE, PREY_REPRODUCTION_CHANCE,
                PREDATOR_HUNGER_LIMIT};

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

    const char *path = "simulation.tsv";
    std::ofstream file(path);
    if (!file) {
        std::cerr << "Unable to open file for writing.\n";
        std::abort();
    }
    file << "time,predators,prey\n";
    vector<int> times;
    vector<int> pred_pops;
    vector<int> prey_pops;
    // Tick equal to num_ticks
    for (int i = 0; i < NUM_TICKS; i++) {
        // Record the predators and prey every tick 
        if (i % 1 == 0) {
            times.emplace_back(i);
            pred_pops.emplace_back(world.num_predators);
            prey_pops.emplace_back(world.num_prey);
        }
        world.tick();
    }
    for (int i = 0; i < times.size(); i++) {
        file << times[i] << "," << pred_pops[i] << "," << prey_pops[i] << "\n";
    }
    if (!file) {
        std::cerr << "Failed to write file contents.\n";
        std::abort();
    }
    file.close();
    return 0;
}
