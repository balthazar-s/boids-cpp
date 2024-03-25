#include <vector>
#include <SFML/Graphics.hpp>
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

    void update_pos(int WIDTH, int HEIGHT);

    void update_pos_avoidwalls(int WIDTH, int HEIGHT);

    void separation(vector<Boid>& boids);

    void alignment_and_cohesion(vector<Boid>& boids);

    void speed_cap();
};

#endif