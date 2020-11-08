#include <algorithm>

#include "Agent.h"
#include "World.h"

using namespace std;

int Agent::id_ = 0; // global counter

Agent::Agent(World* world, bool predator, int x_pos, int y_pos) {
    id = id_++;
    this->x_pos = x_pos;
    this->y_pos = y_pos;
    this->predator = predator;
    this->hunger = 0;
    this->direction = -1;
    this->world = world;
}

// Mark a prey for deletion in the specified direction
vector<Agent>::iterator Agent::eat(int x, int y) {
    int new_x = x_pos + x;
    int new_y = y_pos + y;
    int size = world->grid.size();
    if (new_x < size
        && new_x >= 0
        && new_y < size
        && new_y >= 0) {
        // If there's nothing there, increase hunger
        if (world->grid[x_pos + x][y_pos + y] == -1) {
                    hunger += 1;
        } else {
        vector<Agent>::iterator agentAt = world->getAgentAt(
                x_pos + x, y_pos + y);
            // If there's a prey there, return the iterator for deletion
            if (agentAt->predator == false) {
                hunger = 0;
                return agentAt;
            }
            // Otherwise increase hunger
            hunger += 1;
        }
    }
    return world->agents.end();
}

// Move in the targeted direction
void Agent::move(int x, int y) {
    int new_x;
    int new_y;
    int size = world->grid.size();
    // If we go out of bounds
    if (x_pos + x >= size
        || x_pos + x < 0
        || y_pos + y >= size
        || y_pos + y < 0) {
        // Reverse the velocity
        new_x = x_pos - x;
        new_y = y_pos - y;
        // Update the new position
        if (world->grid[new_x][new_y] == -1) {
            world->placeIDAt(id, new_x, new_y);
            x_pos = new_x;
            y_pos = new_y;
        }
    } else {
        new_x = x_pos + x;
        new_y = y_pos + y;
        // If there's nothing there
        if (world->grid[new_x][new_y] == -1) {
            // Update the new position
            x_pos = new_x;
            y_pos = new_y;
            world->placeIDAt(id, x_pos, y_pos);
        }
    }
}
