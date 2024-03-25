#include <vector>
#include "boids.hpp"
#include "settings.hpp"
#include "grid.hpp"
using namespace std;

void Grid::init(vector<Boid> boids)
{
    grid_size_x = WIDTH / grid_dim;
    grid_size_y = HEIGHT / grid_dim;

    boid_grid.resize(grid_size_x);
    for (int i = 0; i < grid_size_x; i++)
    {
        boid_grid[i].resize(grid_size_y);
    }

    for (int i = 0, len = boids.size(); i < 0; i++)
    {
        
    }
}

void update_grid_pos(Boid this_boid)
{
    vector<int> old_grid_coords = this_boid.grid_coords;
    vector<int> new_grid_coords = calculate_grid_cords(grid_dim, this_boid);



    
}


vector<int> calculate_grid_cords(int cell_size, Boid this_boid)
{
    return {static_cast<int>(this_boid.pos[0]) / cell_size, static_cast<int>(this_boid.pos[1]) / cell_size};
}