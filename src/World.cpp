#include <algorithm>
#include <vector>
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

void World::tick() {
    int delta_x;
    int delta_y;
    vector<vector<Agent>::iterator> toDelete;
    int newPredators = 0;
    int newPrey = 0;

    #pragma omp parallel for
    for (int i = 0; i < agents.size(); i++) {
        Agent *agent = &agents[i];
        vector<vector<Agent>::iterator> localDelete;
        // Directions to move
        agent->direction = (generator() % 4); // Generate a random direction for each agent to attempt to eat/move

        // Determine the direction to eat
        if (agent->predator) {
            // Roll for reproduction
            switch (agent->direction) {
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

            vector<Agent>::iterator eaten = agent->eat(delta_x, delta_y);
            // If there's a prey at the position, mark it for deletion
            if (eaten != agents.end()) {
                localDelete.push_back(eaten);
            }
            // If the predator is starving, mark it for deletion
            if (agent->hunger > predator_hunger_limit) {
                localDelete.push_back(agents.begin() + i);
            }
        }
        #pragma omp critical
        // Iteratively append each thread-local list of to-be-deleted agents to the list
        toDelete.insert(toDelete.end(), localDelete.begin(), localDelete.end());
    }

    // Reverse sort the positions to delete and erase them
    
    sort(toDelete.rbegin(), toDelete.rend());
    for (auto iter : toDelete) {
        agents.erase(iter);
    }

   #pragma omp parallel for
    for (int i = 0; i < agents.size(); i++) {
        Agent *agent = &agents[i];
        // Choose a direction
        switch (agent->direction) {
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
        //#pragma omp critical
        //agent->move(delta_x, delta_y);
    }

    // Check for reproduction
    for (int i = 0; i < agents.size(); i++) {
        int reproduce;
        Agent *agent = &agents[i];

        if (agent->predator) {
            reproduce = spawn_predator();
            newPredators += 1;

        } else {
            reproduce = spawn_prey();
            newPrey += 1;
        }
    }

    // Spawn new predators
    for (int i = 0; i < newPredators; i++) {
        int x_pos = generator() % grid.size();
        int y_pos = generator() % grid.size();
        if (grid[x_pos][y_pos] == -1) {
                agents.emplace_back(this, true, x_pos, y_pos);
                placeIDAt(agents.back().id, agents.back().x_pos, agents.back().x_pos);
                num_predators++;
        }
    }

    // Spawn new prey
    for (int i = 0; i < newPrey; i++) {
        int x_pos = generator() % grid.size();
        int y_pos = generator() % grid.size();
        if (grid[x_pos][y_pos] == -1) {
                agents.emplace_back(this, false, x_pos, y_pos);
                placeIDAt(agents.back().id, agents.back().x_pos, agents.back().x_pos);
                num_predators++;
        }
    }
}

// Roll on whether we spawn a new predator
int World::spawn_predator() {
    if (dis(generator) < predator_reproduction_chance && num_predators > 1) {
        return 1;
    }
    return 0;
}
// Generate a new prey if the probability roll is less than the reproduction rate
int World::spawn_prey() {
    if ((dis(generator) < prey_reproduction_chance && num_prey > 1)) {
        return 1;
    }
    return 0;
}