#include <vector>
#include <SFML/Graphics.hpp>
#include "grid.hpp"
using namespace std;

#ifndef BOIDS_HPP
#define BOIDS_HPP

class Boid {
  public:
    // Public variables
    vector<float> pos;
    vector<float> vel;
    int angle;
    sf::ConvexShape boid_shape;
    vector<int> grid_coords;

    // Constructor to initialize pos and vel
    Boid(const vector<float> position, const vector<float> velocity) : pos(position), vel(velocity) {};

    // Public functions
    void initialise();

    void draw_boid(sf::RenderWindow& window);

    void update_pos();

    void update_pos_avoidwalls();

    void separation_alignment_cohesion(vector<Boid>& boids, Grid& uniform_grid);

    void speed_cap();
};

#endif