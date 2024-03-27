#include "../include/settings.hpp"

// Define variables
int WIDTH;
int HEIGHT;

float maxspeed;
float minspeed;

float avoidfactor;
float protected_range;

float visible_range_alignment;
float visible_range_cohesion;

float matching_factor;

float centering_factor;

bool avoid_walls;
float turnfactor;
bool menu_dropdown;
int margin;
int grid_dim;

// Initialize settings at proper values
void init_settings()
{ 
    WIDTH = 1800;
    HEIGHT = 1000;

    // Speedcap
    maxspeed = 1.9;
    minspeed = 0.5;

    // Separation
    avoidfactor = 0.055;
    protected_range = 10;

    // Alignment and Cohesion
    visible_range_alignment = 60;
    visible_range_cohesion = 30;


    // Alignment
    matching_factor = 0.007;

    // Cohesion
    centering_factor = 0.0011;

    // Other settings
    avoid_walls = false;
    turnfactor = 0.1;
    menu_dropdown = false;
    margin = 50;

    grid_dim = 40;
}
