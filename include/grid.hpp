#include <vector>
#include "boids.hpp"
using namespace std;

#ifndef GRID_HPP
#define GRID_HPP

class Grid
{
    public:
        vector<vector<vector<Boid>>> grid;
}

#endif