#include <vector>
#include "boids.hpp"
#include "settings.hpp"
using namespace std;

#ifndef GRID_HPP
#define GRID_HPP

class Grid
{
    public:
        vector<vector<vector<Boid>>> boid_grid;
        int grid_size_x;// = WIDTH / grid_dim;
        int grid_size_y;// = HEIGHT / grid_dim;

        void init(vector<Boid> boids);

        void update_grid_pos(Boid this_boid);

        vector<int> calculate_grid_cords(int cell_size, Boid this_boid);

        vector<Boid*> get_neighboring_boids(vector<int> grid_coords);
};

#endif