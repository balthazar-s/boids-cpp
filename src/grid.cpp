#include <vector>
#include <algorithm>
#include "boids.hpp"
#include "settings.hpp"
#include "grid.hpp"
using namespace std;

void Grid::init(vector<Boid>& boids)
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
        vector<int> grid_coords = calculate_grid_cords(boids[i]);

        boids[i].grid_coords = grid_coords;
        boid_grid[grid_coords[0]][grid_coords[1]].push_back(&boids[i]);
    }
}


void Grid::update_grid_pos(Boid& this_boid)
{
    vector<int> old_c = this_boid.grid_coords;
    vector<int> new_c = calculate_grid_cords(this_boid);

    auto it = find(boid_grid[old_c[0]][old_c[1]].begin(), boid_grid[old_c[0]][old_c[1]].end(), &this_boid);

    boid_grid[old_c[0]][old_c[1]].erase(it);

    boid_grid[new_c[0]][new_c[1]].push_back(&this_boid);
    
}


vector<int> Grid::calculate_grid_cords(Boid& this_boid)
{   
    int cell_size = grid_dim;
    return {static_cast<int>(this_boid.pos[0]) / cell_size, static_cast<int>(this_boid.pos[1]) / cell_size};
}


vector<Boid*> Grid::get_neighboring_boids(vector<int> grid_coords)
{
    vector<Boid*> neighbors;

    // Iterate over neighboring cells
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int x = grid_coords[0] + dx;
            int y = grid_coords[1] + dy;

            // Check if the cell is within the grid bounds
            if (x >= 0 && x < boid_grid.size() && y >= 0 && y < boid_grid[0].size()) {
                // Add boids from the neighboring cell to the list of neighbors
                for (Boid* neighbor_boid : boid_grid[x][y]) {
                    neighbors.push_back(neighbor_boid);
                }
            }
        }
    }

    return neighbors;
}