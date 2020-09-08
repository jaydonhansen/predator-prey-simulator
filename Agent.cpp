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
    this->world = world;
}

// Attempt to eat something at the targeted point
int Agent::eat(int x, int y) {
    int new_x = x_pos + x;
    int new_y = y_pos + y;
    int size = world->grid.size();
    if (this->predator) {
        if (new_x < size
            && new_x >= 0
            && new_y < size
            && new_y >= 0) {
            vector<Agent>::iterator agentAt;
            if ((agentAt = world->getAgentAt(
                    x_pos + x, y_pos + y)) == world->agents.end()) {
            } else {
                if (agentAt->predator == false) {
                    int dist = distance(begin(world->agents), agentAt);
                    world->removeIDAt(new_x, new_y);
                    world->agents.erase(agentAt);
                    world->num_prey--;
                    return dist;
                }
            }
        }
    }
    return -1;
}

// Move in the targeted direction
void Agent::move(int x, int y) {
    int new_x;
    int new_y;
    int size = world->grid.size();
    if (x_pos + x >= size
        || x_pos + x < 0
        || y_pos + y >= size
        || y_pos + y < 0) {
        // Reverse the velocity
        new_x = x_pos - x;
        new_y = y_pos - y;
        // Update the new position
        if (world->getAgentAt(new_x, new_y) == world->agents.end()) {
            world->placeIDAt(id, new_x, new_y);
            x_pos = new_x;
            y_pos = new_y;
        }
    } else {
        new_x = x_pos + x;
        new_y = y_pos + y;
        // If there's nothing there
        if (world->getAgentAt(new_x, new_y) == world->agents.end()) {
            // Update the new position
            x_pos = new_x;
            y_pos = new_y;
            world->placeIDAt(id, x_pos, y_pos);
        }
    }
}
