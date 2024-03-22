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
    //sf::ConvexShape boid_shape;
    sf::CircleShape boid_shape;

    // Constructor to initialize pos and vel
    Boid(const vector<float> position, const vector<float> velocity) : pos(position), vel(velocity) {};

    // Public functions
    void initialise();

    void draw_boid(sf::RenderWindow& window);

    void update_pos(int WIDTH, int HEIGHT, vector<Boid>& boids);

    void speed_cap();
};

#endif