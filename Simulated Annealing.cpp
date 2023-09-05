#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <iostream>
#include <random>

int Width = 1280;
int Height = 720;

const int NumRows = 14;
const int NumColumns = 25;
const float CellSize = 51.1f;

struct City
{
    int x;
    int y;
};

double calculateDistance(const City& city1, const City& city2)
{
    int dx = city2.x - city1.x;
    int dy = city2.y - city1.y;
    return std::sqrt(dx * dx + dy * dy);
}

double calcdist(const std::vector<City>& points)
{
    double sum = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        double d = calculateDistance(points[i], points[i + 1]);
        sum += d;
    }
    return sum;
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML TSP with Simulated Annealing", sf::Style::Default, settings);
    window.setFramerateLimit(120);
    sf::Clock clock;

    sf::Color red(235, 69, 95);
    sf::Color green(13, 84, 93);

    sf::VertexArray grid(sf::Lines);

    // Create the vertical lines of the grid
    for (int col = 0; col <= NumColumns; ++col) {
        float x = col * CellSize;
        grid.append(sf::Vertex(sf::Vector2f(x, 0.f), green));
        grid.append(sf::Vertex(sf::Vector2f(x, NumRows * CellSize), green));
    }

    // Create the horizontal lines of the grid
    for (int row = 0; row <= NumRows; ++row) {
        float y = row * CellSize;
        grid.append(sf::Vertex(sf::Vector2f(0.f, y), green));
        grid.append(sf::Vertex(sf::Vector2f(NumColumns * CellSize, y), green));
    }

    std::vector<City> cities = {
        {100, 500},
        {200, 300},
        {400, 200},
        {600, 350},
        {500, 400},
        {170, 250},
        {120, 630},
    };

    std::vector<sf::CircleShape> circles;

    for (const auto& city : cities) {
        sf::CircleShape circle(10);
        circle.setPosition(city.x - 10, city.y - 10);
        circle.setFillColor(sf::Color::White);
        circles.push_back(circle);
    }

    sf::RectangleShape lineSegment;

    double recordDistance = calcdist(cities);
    std::vector<City> bestEver = cities;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(0, cities.size() - 1);

    double initialTemperature = 1000.0;
    double coolingRate = 0.995;
    int maxIterations = 50000;

    while (window.isOpen() && maxIterations > 0)
    {
        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(grid);

        // Store a copy of the current solution for potential reversion
        std::vector<City> currentSolution = cities;

        // Randomly select two indices for swapping
        int i = distribution(generator);
        int j = distribution(generator);

        // Make sure i and j are distinct
        while (i == j) {
            j = distribution(generator);
        }

        // Swap the cities in the current solution
        std::swap(currentSolution[i], currentSolution[j]);

        // Calculate distances for the current and best solutions
        double currentDistance = calcdist(currentSolution);

        // Calculate the change in distance (delta)
        double delta = currentDistance - calcdist(cities);

        // Check if the new solution is accepted based on a probabilistic criterion
        if (delta < 0 || (rand() / (RAND_MAX + 1.0)) < exp(-delta / initialTemperature)) {
            cities = currentSolution;  // Accept the new solution
            if (currentDistance < recordDistance) {
                recordDistance = currentDistance;
                bestEver = cities;
                std::cout << "Record Distance is " << recordDistance << std::endl;
            }
        }

        // Draw the tour for the current solution
        for (int i = 0; i < cities.size(); i++) {
            int nextIndex = (i + 1) % cities.size();
            lineSegment.setSize(sf::Vector2f(calculateDistance(cities[i], cities[nextIndex]), 1.5f));
            lineSegment.setFillColor(sf::Color::Green);
            lineSegment.setPosition(sf::Vector2f(cities[i].x, cities[i].y));
            lineSegment.setRotation(std::atan2(cities[nextIndex].y - cities[i].y, cities[nextIndex].x - cities[i].x) * 180.f / 3.14159265f);
            window.draw(lineSegment);
            window.draw(circles[i]);
        }

        // Draw the tour for the best solution found so far
        for (int i = 0; i < bestEver.size(); i++) {
            int nextIndexx = (i + 1) % bestEver.size();
            lineSegment.setSize(sf::Vector2f(calculateDistance(bestEver[i], bestEver[nextIndexx]), 3.0f));
            lineSegment.setFillColor(sf::Color::Blue);
            lineSegment.setPosition(sf::Vector2f(bestEver[i].x, bestEver[i].y));
            lineSegment.setRotation(std::atan2(bestEver[nextIndexx].y - bestEver[i].y, bestEver[nextIndexx].x - bestEver[i].x) * 180.f / 3.14159265f);
            window.draw(lineSegment);
        }

        window.display();
        maxIterations--;

        // Reduce the initial temperature
        initialTemperature *= coolingRate;
    }

    return 0;
}

