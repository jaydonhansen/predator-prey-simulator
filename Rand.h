#pragma once
#include <random>

// Generates a seed from the current time then generates a random integer
static unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
static std::mt19937 generator(seed);
static std::uniform_real_distribution<double> dis(0.0, 1.0);

