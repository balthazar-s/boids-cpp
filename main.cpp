#include <SFML/Graphics.hpp> // Graphics library
#include "include/boids.hpp" // Boid class
#include "include/settings.hpp"
#include <vector> // For vector lists
#include <cstdlib> // For Random number generation
#include <random>
using namespace std;

void cleanupBoids(vector<vector<vector<Boid*>>>& boids);

int main()
{
    init_settings();

    // Window variables
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8; // Adjust the antialiasing level as needed
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Boids", sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);

    srand((unsigned) time(NULL));

    // Define number of boids
    int cols = 20;
    int rows = 20;

    // Create all Boids
    vector<vector<vector<Boid*>>> boids(grid_sizex);
    for (int i = 0; i < grid_sizex; i++) 
    {
        boids[i].resize(grid_sizey);
    }

    // Define a random number generator engine
    random_device rd;
    mt19937 gen(rd());

    for (int i = 0; i < rows; i++)
    {   
        for (int j = 0; j < cols; j++)
        {   
            // Grid for coordinates
            float cord_x = (WIDTH/(cols+1))*(j+1);
            float cord_y = (HEIGHT/(rows+1))*(i+1);

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
            

            // Create new Boid object dynamically and store its pointer in the list
            Boid* new_boid = new Boid(position, velocity);
            new_boid->initialise();
            boids[i][j].push_back(new_boid);
        }
    }
            
    // Simulation variables
    const int SIMULATION_FPS = 60;
    const sf::Time SIMULATION_TIME_PER_FRAME = sf::seconds(1.0f / SIMULATION_FPS);
    sf::Clock simulationClock;
    sf::Time elapsedTimeSinceLastUpdate = sf::Time::Zero;

    // Main process
    while (window.isOpen()) {
        // Exit window event
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                cleanupBoids(boids);
                window.close();
        }

        // Simulate fixed time steps
        elapsedTimeSinceLastUpdate += simulationClock.restart();
        while (elapsedTimeSinceLastUpdate >= SIMULATION_TIME_PER_FRAME) {
            // Update simulation
            for (int i = 0, len = boids.size(); i < len; i++) {
                for (int j = 0, len_2 = boids[i].size(); j < len_2; j++)
                {
                    for (int k = 0, len_3 = boids[i][j].size(); k < len_3; k++)
                    {
                        boids[i][j][k]->update_pos(WIDTH, HEIGHT, boids);
                        boids[i][j][k]->speed_cap(); 
                    }
                }
                
            }
            elapsedTimeSinceLastUpdate -= SIMULATION_TIME_PER_FRAME;
        }
        window.clear();

        // Draw all Boids
        for (int i = 0, len = boids.size(); i < len; i++) 
        {
            for (int j = 0, len_2 = boids[i].size(); j < len_2; j++)
            {
                for (int k = 0, len_3 = boids[i][j].size(); k < 0; k++)
                {
                    boids[i][j][k]->draw_boid(window);
                }
            }
            
        }

        window.display();
    }

    return 0;
}

// Function to clean up memory used by the boids vector
void cleanupBoids(vector<vector<vector<Boid*>>>& boids) {
    for (auto& gridRow : boids) {
        for (auto& row : gridRow) {
            for (auto boidPtr : row) {
                delete boidPtr; // Free memory for each Boid object
            }
            row.clear(); // Clear the row vector
        }
        gridRow.clear(); // Clear the gridRow vector
    }
    boids.clear(); // Clear the boids vector
}