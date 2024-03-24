#include "../include/settings.hpp"

// Define variables
int WIDTH;
int HEIGHT;

float maxspeed;
float minspeed;

float avoidfactor;
float protected_range;

float visible_range;

float matching_factor;

float centering_factor;

bool avoid_walls;
float turnfactor;
bool menu_dropdown;
int margin;
int grid_dim;
int grid_size_x;
int grid_size_y;


// Initialize settings at proper values
void init_settings()
{ 
    WIDTH = 1000;
    HEIGHT = 1000;

    // Speedcap
    maxspeed = 3;
    minspeed = 1.5;

    // Separation
    avoidfactor = 0.05;
    protected_range = 12;

    // Alignment and Cohesion
    visible_range = 32;

    // Alignment
    matching_factor = 0.0075;

    // Cohesion
    centering_factor = 0.001;

    // Other settings
    avoid_walls = false;
    turnfactor = 0.1;
    menu_dropdown = false;
    margin = 100;

    grid_dim = 40;
    grid_size_x = WIDTH / grid_dim;
    grid_size_y = HEIGHT / grid_dim;
}
