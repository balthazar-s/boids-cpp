#include <vector> 
#include <math.h>
#include "../include/boids.hpp"
#include "../include/settings.hpp"
using namespace std;

void Boid::initialise()
{
    boid_shape.setPointCount(3);
    boid_shape.setFillColor(sf::Color(255, 255, 255));
    boid_shape.setPoint(0, sf::Vector2f(0, 10)); // Top point
    boid_shape.setPoint(1, sf::Vector2f(-5, -10)); // Bottom left point
    boid_shape.setPoint(2, sf::Vector2f(5, -10)); // Bottom right point
}

void Boid::draw_boid(sf::RenderWindow& window) 
{
    window.draw(boid_shape);
}

void Boid::update_pos(int WIDTH, int HEIGHT, vector<Boid>& boids)
{
    pos[0] += vel[0]; 
    pos[1] += vel[1];

    // Wrap around from edges
    if (pos[0] > WIDTH + 10)
    {
        pos[0] = -10.0;
    }
    else if (pos[1] > HEIGHT + 10)
    {
        pos[1] = -10.0;
    }
    else if (pos[0] < -10)
    {
        pos[0] = WIDTH + 10;
    }
    else if (pos[1] < -10)
    {
        pos[1] = HEIGHT + 10;
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

    // Three rules

    float close_dx = 0.0;
    float close_dy = 0.0;

    float xvel_avg = 0.0;
    float yvel_avg = 0.0;
    int neighboring_boids = 0;

    float xpos_avg = 0.0;
    float ypos_avg = 0.0;

    for (int i = 0, len = boids.size(); i < len; i++)
    {
        // Calculate the squared distance between this boid and the current boid
        float dx = pos[0] - boids[i].pos[0];
        float dy = pos[1] - boids[i].pos[1];
        float distance = sqrt(dx * dx + dy * dy);

        if (distance <= protected_range && distance > 0)
        {
            float inv_distance = 1.0 / distance;
            
            // Scale the repulsion force using the inverse distance
            close_dx += dx * inv_distance;
            close_dy += dy * inv_distance;

            // Calculate the adjustment to the velocity based on separation
            xvel_avg += boids[i].vel[0];
            yvel_avg += boids[i].vel[1];
            neighboring_boids += 1;

            // Add the x and y positions of the other boid to xpos_avg and ypos_avg
            xpos_avg += boids[i].pos[0];
            ypos_avg += boids[i].pos[1];
        } 
    }
 
    if (neighboring_boids > 0)
    {
        xvel_avg = xvel_avg / float(neighboring_boids);
        yvel_avg = yvel_avg / float(neighboring_boids);

        vel[0] += (xvel_avg - vel[0]) * matching_factor;
        vel[1] += (yvel_avg - vel[1]) * matching_factor;

        // Calculate the average position
        xpos_avg /= float(neighboring_boids);
        ypos_avg /= float(neighboring_boids);

        // Update the velocity according to the difference between the average position and the current position
        vel[0] += (xpos_avg - pos[0]) * centering_factor;
        vel[1] += (ypos_avg - pos[1]) * centering_factor;    
    }

    vel[0] += close_dx * avoidfactor;
    vel[1] += close_dy * avoidfactor;

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
