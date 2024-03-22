#include <vector> 
#include <math.h>
#include "../include/boids.hpp"
#include "../include/settings.hpp"
using namespace std;

void Boid::initialise()
{
    boid_shape.setPointCount(3);
    //boid_shape.setFillColor(sf::Color(255, 255, 255));
    //boid_shape.setRadius(3);
    //boid_shape.setPointCount(3);
    boid_shape.setPoint(0, sf::Vector2f(0, 5)); // Top point
    boid_shape.setPoint(1, sf::Vector2f(-3, -5)); // Bottom left point
    boid_shape.setPoint(2, sf::Vector2f(3, -5)); // Bottom right point
}

void Boid::draw_boid(sf::RenderWindow& window) 
{
    window.draw(boid_shape);
}

void Boid::update_pos(int WIDTH, int HEIGHT, vector<vector<vector<Boid*>>>& boids)
{
    // Update position
    pos[0] += vel[0]; 
    pos[1] += vel[1];

    // Check if the boid hits the right or left wall
    if (pos[0] > WIDTH - margin)
    {
        vel[0] -= turnfactor;    
    }
    else if (pos[0] < margin)
    {
        vel[0] += turnfactor;    
    }
    // Check if the boid hits the bottom or top wall
    if (pos[1] > HEIGHT - margin)
    {
        vel[1] -= turnfactor;       
    }
    else if (pos[1] < margin)
    {
        vel[1] += turnfactor;       
    }


    
    boid_shape.setPosition(pos[0], pos[1]);
    
    if (vel[0] != 0 || vel[1] != 0) 
    {
        angle = (atan2(vel[1], vel[0]) * 180 / M_PI) - 90;
    }
    else
    {
        angle = 0; 
    }
    boid_shape.setRotation(angle);
    
    // Get current boid grid coords
    vector<int> grid_coords = calculate_grid_cords(40);
    
    // Get neighboring boids
    vector<Boid*> neighbors = get_neighboring_boids(boids, grid_coords);


    // If no neighbors
    if (neighbors.size() == 0)
    {
        return;
    }

    int neighbors_number = neighbors.size();

    // Separation

    float close_dx = 0.0;
    float close_dy = 0.0;
    
    for (int i = 0; i < neighbors_number; i++)
    {   
        // Calculate quared distance between this boid and the current boid
        float dx = pos[0] - neighbors[i]->pos[0];
        float dy = pos[1] - neighbors[i]->pos[1];
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= protected_range && distance > 0)
        {
            float inv_distance = 1.0 / distance;
            
            // Scale the repulsion force using the inverse distance
            close_dx += dx * inv_distance;
            close_dy += dy * inv_distance;
        } 
    }

    vel[0] += close_dx * avoidfactor;
    vel[1] += close_dy * avoidfactor;
    
    // Alignment and Cohesion

    float xvel_avg = 0.0;
    float yvel_avg = 0.0;
    float xpos_avg = 0.0;
    float ypos_avg = 0.0;

    for (int i = 0; i < neighbors_number; i++)
    {    
        // Calculate the distance between this boid and the current boid
        float dx = neighbors[i]->pos[0] - pos[0];
        float dy = neighbors[i]->pos[1] - pos[1];
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= visible_range && distance > 0)
        {
            // Calculate the adjustment to the velocity based on separation
            xvel_avg += neighbors[i]->vel[0];
            yvel_avg += neighbors[i]->vel[1];

            // Add the x and y positions of the other boid to xpos_avg and ypos_avg
            xpos_avg += neighbors[i]->pos[0];
            ypos_avg += neighbors[i]->pos[1];
        }
    }   
     
    if (neighbors_number > 0)
    {
        xvel_avg /= float(neighbors_number);
        yvel_avg /= float(neighbors_number);

        vel[0] += (xvel_avg - vel[0]) * matching_factor;
        vel[1] += (yvel_avg - vel[1]) * matching_factor;

        // Calculate the center of mass
        float center_x = xpos_avg / neighbors_number;
        float center_y = ypos_avg / neighbors_number;

        // Calculate the direction towards the center
        float dx_center = center_x - pos[0];
        float dy_center = center_y - pos[1];

        // Update the velocity according to the difference between the current position and the center
        vel[0] += dx_center * centering_factor;
        vel[1] += dy_center * centering_factor;   
    }    
    
    // Update color
    /*
    int color_level = round(neighboring_boids * 255 / 16);

    if (color_level > 255)
    {
        color_level = 255;
    }

    boid_shape.setFillColor(sf::Color(color_level, color_level, color_level));
    */
}

void Boid::speed_cap()
{
    float speed = sqrt(vel[0] * vel[0] + vel[1] * vel[1]);

    if (speed > maxspeed)
    {
        vel[0] = (vel[0] / speed) * maxspeed;
        vel[1] = (vel[1] / speed) * maxspeed;
    }

    if (speed < minspeed)
    {
        vel[0] = (vel[0] / speed) * minspeed;
        vel[1] = (vel[1] / speed) * minspeed;
    }
}

vector<int> Boid::calculate_grid_cords(int cell_size)
{
    return {static_cast<int>(pos[0]) / cell_size, static_cast<int>(pos[1]) / cell_size};
}

vector<Boid*> Boid::get_neighboring_boids(const vector<vector<vector<Boid*>>>& boids, vector<int> grid_coords)
{
    vector<Boid*> neighbors;

    // Iterate over neighboring cells
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            int x = grid_cords[0] + dx;
            int y = grid_cords[1] + dy;

            // Check if the cell is within the grid bounds
            if (x >= 0 && x < boids.size() && y >= 0 && y < boids[0].size()) {
                // Add boids from the neighboring cell to the list of neighbors
                for (Boid* neighbor : boids[x][y]) {
                    neighbors.push_back(neighbor);
                }
            }
        }
    }

    return neighbors;
}
