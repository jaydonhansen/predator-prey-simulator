#include <vector>
#include "Rand.h"
#include "World.h"
using namespace std;

World::World(int size): grid(size, vector<int>(size, -1)) {
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
    int remaining = agents.size();
    for (int i = 0; i < remaining; i++) {
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
        int eaten;
        eaten = agents[i].eat(delta_x, delta_y);
        if (eaten == -1) {

        } else if (eaten < i) {
            i--;
            remaining = agents.size();
        } else {
            remaining = agents.size();
        }
        agents[i].move(delta_x, delta_y);
    }
}
