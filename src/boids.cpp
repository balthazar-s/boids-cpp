#include <vector> 
#include <math.h>
#include "../include/boids.hpp"
#include "../include/settings.hpp"
#include "../include/grid.hpp"
using namespace std;

void Boid::initialise()
{
    boid_shape.setPointCount(3);
    boid_shape.setFillColor(sf::Color(255, 255, 255));
    boid_shape.setPoint(0, sf::Vector2f(0, 2)); // Top point
    boid_shape.setPoint(1, sf::Vector2f(-1, -2)); // Bottom left point
    boid_shape.setPoint(2, sf::Vector2f(1, -2)); // Bottom right point
    grid_coords = {0, 0};
}

void Boid::draw_boid(sf::RenderWindow& window) 
{
    window.draw(boid_shape);
}

void Boid::update_pos()
{
    pos[0] += vel[0]; 
    pos[1] += vel[1];

    // Wrap around from edges
    if (pos[0] > WIDTH-10)
    {
        pos[0] = 10;
    }
    else if (pos[1] > HEIGHT-10)
    {
        pos[1] = 10;
    }
    else if (pos[0] < 10)
    {
        pos[0] = WIDTH-10;
    }
    else if (pos[1] < 10)
    {
        pos[1] = HEIGHT-10;
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
}

void Boid::update_pos_avoidwalls()
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


    // Update the position of the boid shape
    boid_shape.setPosition(pos[0], pos[1]);

    // Cosmetic rotation
    if (vel[0] != 0 || vel[1] != 0) 
    {
        angle = (atan2(vel[1], vel[0]) * 180 / M_PI) - 90;
    }
    else
    {
        angle = 0; 
    }
    boid_shape.setRotation(angle);
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

void Boid::separation_alignment_cohesion(vector<Boid>& boids, Grid& uniform_grid)
{   
    vector<Boid*> neighbors = uniform_grid.get_neighboring_boids(grid_coords);
    
    float close_dx = 0.0;
    float close_dy = 0.0;

    for (int i = 0, len = neighbors.size(); i < len; i++)
    {
        // Calculate the squared distance between this boid and the current boid
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

    float xvel_avg = 0.0;
    float yvel_avg = 0.0;
    float xpos_avg = 0.0;
    float ypos_avg = 0.0;
    int neighboring_boids_alignment = 0;
    int neighboring_boids_cohesion = 0;


    for (int i = 0, len = neighbors.size(); i < len; i++)
    {
        // Calculate the distance between this boid and the current boid
        float dx = neighbors[i]->pos[0] - pos[0];
        float dy = neighbors[i]->pos[1] - pos[1];
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= 0)
        {
            return;
        }

        if (distance <= visible_range_alignment)
        {
            // Calculate the adjustment to the velocity based on separation
            xvel_avg += neighbors[i]->vel[0];
            yvel_avg += neighbors[i]->vel[1];
            neighboring_boids_alignment += 1;
        }
        if (distance <= visible_range_cohesion)
        {
            // Add the x and y positions of the other boid to xpos_avg and ypos_avg
            xpos_avg += neighbors[i]->pos[0];
            ypos_avg += neighbors[i]->pos[1];
            neighboring_boids_cohesion += 1;
        }
    }    
    if (neighboring_boids_alignment > 0)
    {
        xvel_avg = xvel_avg / float(neighboring_boids_alignment);
        yvel_avg = yvel_avg / float(neighboring_boids_alignment);

        vel[0] += (xvel_avg - vel[0]) * matching_factor;
        vel[1] += (yvel_avg - vel[1]) * matching_factor;  
    }    
    if (neighboring_boids_cohesion > 0)
    {
        xpos_avg /= float(neighboring_boids_cohesion);
        ypos_avg /= float(neighboring_boids_cohesion);

        vel[0] += (xpos_avg - pos[0]) * centering_factor;
        vel[1] += (ypos_avg - pos[1]) * centering_factor;  
    }
}