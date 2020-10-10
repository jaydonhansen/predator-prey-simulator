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
    kill_predators();
    int delta_x;
    int delta_y;
    int remaining = agents.size();
    for (int i = 0; i < remaining; i++) {
        Agent *agent = &agents[i];
        // Directions to move
        // Generate a random int [0..3], then decide a new trajectory
        int direction = generator() % 4;
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
        // If the agent is a predator, call eat()
        if (agent->predator) {
            int eaten;
            eaten = agent->eat(delta_x, delta_y);
            if (eaten == -1) {
                // If nothing is eaten, increment hunger
                agent->hunger += 1;
                // If hunger > 5, kill the hunter
            } else if (eaten < i) {
                agent->hunger = 0;
                i--;
                remaining = agents.size();
            } else {
                agent->hunger = 0;
                remaining = agents.size();
            }
        }
        agent->move(delta_x, delta_y);
        if (agent->predator) {
            spawn_predator();
        } else {
            spawn_prey();
        }
    }
}

// Generate a new predator with a probability
void World::spawn_predator() {
    if (dis(generator) < predator_reproduction_chance && num_predators > 1) {
        int x_pos = generator() % grid.size();
        int y_pos = generator() % grid.size();

        if (grid[x_pos][y_pos] == -1) {
            agents.emplace_back(this, true, x_pos, y_pos);
            placeIDAt(agents.back().id, agents.back().x_pos, agents.back().x_pos);
            num_predators++;
        }
    }

}
// Generate a new prey if the probability roll is less than the reproduction rate
void World::spawn_prey() {
    if ((dis(generator) < prey_reproduction_chance && num_prey > 1)) {
        int x_pos = generator() % grid.size();
        int y_pos = generator() % grid.size();

        if (grid[x_pos][y_pos] == -1) {
            agents.emplace_back(this, false, x_pos, y_pos);
            placeIDAt(agents.back().id, agents.back().x_pos, agents.back().x_pos);
            num_prey++;
        }
    }
}

void World::kill_predators() {
    for (int i = 0; i < agents.size(); i++) {
        Agent* agent = &agents[i];
        if (agent->predator) {
           if (agent->hunger >= predator_hunger_limit) {
               removeIDAt(agent->x_pos, agent->y_pos);
               agents.erase(agents.begin() + (agent - agents.data()));
               num_predators--;
           }
        }
    }
}

