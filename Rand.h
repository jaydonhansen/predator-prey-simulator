#pragma once
#include <random>
using namespace std;

// Generates a seed from the current time then generates a random integer
static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
static std::mt19937 generator(seed);