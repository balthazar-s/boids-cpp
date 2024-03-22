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
    //sf::CircleShape boid_shape;
    vector<int> grid_cords;

    // Constructor to initialize pos and vel
    Boid(const vector<float> position, const vector<float> velocity) : pos(position), vel(velocity), grid_cords(2) {};

    // Public functions
    void initialise();

    void draw_boid(sf::RenderWindow& window);

    void update_pos(int WIDTH, int HEIGHT, vector<vector<vector<Boid*>>>& boids);

    void speed_cap();

    vector<int> calculate_grid_cords(int cell_size);

    vector<Boid*> Boid::get_neighboring_boids(const vector<vector<vector<Boid*>>>& boids, vector<int> grid_coords)
};

#endif