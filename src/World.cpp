#include <vector>
#include <algorithm>
#include <iostream>
#include "Rand.h"
#include "World.h"
using namespace std;


World::World(int size, int initial_predators, int initial_prey,
             float predator_reproduction_chance, float prey_reproduction_chance,
             int predator_hunger_limit):
             grid(size, vector<int>(size, -1)) {
    num_predators = initial_predators;
    num_prey = initial_prey;
    this->predator_reproduction_chance = predator_reproduction_chance;
    this->prey_reproduction_chance = prey_reproduction_chance;
    this->predator_hunger_limit = predator_hunger_limit;
}

vector<Agent>::iterator World::getAgentAt(int x, int y) {
    int id = grid[x][y];
    if (id == -1) {
        return agents.end();
    }
    auto found = find_if(begin(agents),
                         end(agents),
                         [id](Agent const& agent)
                         { return agent.id == id;});
    return found;
}

void World::placeIDAt(int id, int x, int y) {
    grid[x][y] = id;
}

void World::removeIDAt(int x, int y) {
    grid[x][y] = -1;
}

// The basic iterative process of the game
void World::tick() {
    int delta_x;
    int delta_y;
    vector<int> toDelete;
    vector<tuple<int, int>> toClear;
    int newPredators = 0;
    int newPrey = 0;

    #pragma omp parallel default(none) shared(toDelete, toClear, delta_x, delta_y, generator)
    #pragma omp for
    for (int i = 0; i < agents.size(); i++) {
        Agent *agent = &agents[i];
        int direction = generator() % 4; // Generate a random direction
        // Predator eat step
        if (agent->predator) { 
        // Determine the direction to eat
            switch (direction) {
                case 0:
                    // North
                    delta_x = 0;
                    delta_y = 1;
                    break;
                case 1:
                    // East
                    delta_x = 1;
                    delta_y = 0;
                    break;
                case 2:
                    // South
                    delta_x = 0;
                    delta_y = -1;
                    break;
                case 3:
                    // West
                    delta_x = -1;
                    delta_y = 0;
                    break;
                default:
                    delta_y = 0;
                    delta_x = 0;
            }
            // It takes energy to go hunting
            int eaten = agent->eat(delta_x, delta_y);
            // If there's a prey at the position, mark it for deletion
            
            #pragma omp critical
            if (eaten != -1 && num_prey > 0) {
                agent->hunger = 0; // yum
                // mark the prey for deletion
                toDelete.push_back(eaten);
                // Calcualte positions to clear
                int new_x = agent->x_pos + delta_x;
                int new_y = agent->y_pos + delta_x;
                // Out of bounds check. This will segfault if not done!
                if (new_x < grid.size()
                    && new_x >= 0
                    && new_y < grid.size()
                    && new_y >= 0) 
                toClear.push_back(std::make_tuple(new_x, new_y)); // Add to the list of agents to remove from the board (ids)
                num_prey -= 1;
            }
            #pragma omp critical
            // If the predator is starving, mark it for deletion
            if (agent->hunger > predator_hunger_limit && num_predators > 0) {
                toDelete.push_back(agent->id);
                toClear.push_back(std::make_tuple(agent->x_pos, agent->y_pos));
                num_predators -= 1; 
            }
        }
    }
    // Remove agents if their ID matches the IDs to be removed
    for (int id : toDelete) {
        agents.erase(remove_if(agents.begin(),
                agents.end(),
                [id](Agent const& agent)
                { return agent.id == id;}),
                agents.end());
    }
    for (tuple<int, int> tup : toClear) {
        grid[get<0>(tup)][get<1>(tup)] = -1;
    }

    #pragma omp for
    for (int i = 0; i < agents.size(); i++) {
       Agent *agent = &agents[i];
       int direction = generator() % 4; // Generate a random direction for each agent to attempt to eat/move
        // Determine the direction to move
        switch (direction) {
            case 0:
                // North
                delta_x = 0;
                delta_y = 1;
                break;
            case 1:
                // East
                delta_x = 1;
                delta_y = 0;
                break;
            case 2:
                // South
                delta_x = 0;
                delta_y = -1;
                break;
            case 3:
                // West
                delta_x = -1;
                delta_y = 0;
                break;
            default:
                delta_y = 0;
                delta_x = 0;
        }
        #pragma omp critical
        agent->move(delta_x, delta_y);
    }

    #pragma omp for
    for (int i = 0; i < num_predators; i++) {
        if ((dis(generator) < predator_reproduction_chance && num_predators > 1)) {
            #pragma omp critical
            newPredators++;
        }
    }

    #pragma omp for
    for (int i = 0; i < num_prey; i++) {
        if ((dis(generator) < prey_reproduction_chance && num_prey > 1)) {
            #pragma omp critical
            newPrey++;
       }
    }

    // Spawn and place IDs of new predators
    for (int i = 0; i < newPredators; i++) {
        spawn_predator();
    }
    for (auto const& agent : agents) {
        placeIDAt(agent.id, agent.x_pos, agent.y_pos);
    }

    // Spawn and place IDs of new prey
    for (int i = 0; i < newPrey; i++) {
        spawn_prey();
    }

    for (auto const& agent : agents) {
        placeIDAt(agent.id, agent.x_pos, agent.y_pos);
    }

}

// Generate a new prey if the probability roll is less than the reproduction rate

void World::spawn_prey() {
    int x_pos = generator() % grid.size();
    int y_pos = generator() % grid.size();

    if (grid[x_pos][y_pos] == -1) {
        agents.emplace_back(this, false, x_pos, y_pos);
        num_prey++;
    }
}

void World::spawn_predator() {
    int x_pos = generator() % grid.size();
    int y_pos = generator() % grid.size();

    if (grid[x_pos][y_pos] == -1) {
        agents.emplace_back(this, true, x_pos, y_pos);
        num_predators++;
    }
}