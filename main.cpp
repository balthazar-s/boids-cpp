#include <SFML/Graphics.hpp> // Graphics library
#include "include/boids.hpp" // Boid class
#include "include/settings.hpp" // Settings to facilitate tweaking values
#include <vector> // For vector lists
#include <cstdlib> // For Random number generation
#include <random>
using namespace std;

int main()
{   
    // Initialise all settings in settings.cpp
    init_settings();

    // Window variables
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Boids", sf::Style::Titlebar | sf::Style::Close, settings);
    
    // Random number seed
    srand((unsigned) time(NULL));

    // Create all Boids
    vector<Boid> boids;

    // Define a random number generator engine
    random_device rd;
    mt19937 gen(rd());

    // Across
    for (int i = 0; i < boids_cols; i++)
    {
        // Down
        for (int j = 0; j < boids_rows; j++)
        {   
            // Grid for coordinates
            float cord_x = (WIDTH/(boids_rows+1))*(j+1);
            float cord_y = (HEIGHT/(boids_cols+1))*(i+1);

            // Random velocities
            uniform_real_distribution<float> vel_dis(minspeed, maxspeed);
            float vel_x = vel_dis(gen);
            float vel_y = vel_dis(gen);

            // Randomize sign of velocities
            uniform_int_distribution<int> sign_dis(0, 1);
            bool negative = sign_dis(gen);
            if (negative == true) 
            {
                vel_x = -vel_x;
            }
            negative = sign_dis(gen);
            if (negative == true) 
            {
                vel_y = -vel_y;
            }

            // Create vectors to be used in boid creation
            vector<float> position = {cord_x, cord_y};
            vector<float> velocity = {vel_x, vel_y};
            
            // Create new Boid object at end of boids list
            boids.push_back(Boid(position, velocity));
            boids.back().initialise();
        }
    }
            
    // Simulation variables
    // This is done seperately from the window so as to avoid frame stuttering
    const int SIMULATION_FPS = 80;
    const sf::Time SIMULATION_TIME_PER_FRAME = sf::seconds(1.0f / SIMULATION_FPS);
    sf::Clock simulationClock;
    sf::Time elapsedTimeSinceLastUpdate = sf::Time::Zero;

    // Main process
    while (window.isOpen()) {
        // Exit window event
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        // Simulate fixed time steps
        elapsedTimeSinceLastUpdate += simulationClock.restart();
        while (elapsedTimeSinceLastUpdate >= SIMULATION_TIME_PER_FRAME) {
            // Update simulation
            for (int i = 0, len = boids.size(); i < len; i++) {
                boids[i].update_pos_avoidwalls(WIDTH, HEIGHT);
                boids[i].separation(boids);
                boids[i].alignment_and_cohesion(boids);
                boids[i].speed_cap();
            }
            elapsedTimeSinceLastUpdate -= SIMULATION_TIME_PER_FRAME;
        }

        // Clear window
        window.clear();

        // Draw all Boids
        for (int i = 0, len = boids.size(); i < len; i++) {
            boids[i].draw_boid(window);
        }

        // Display window
        window.display();
    }

    return 0;
}
