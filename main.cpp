#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>

#include <cstdlib>
#include <ctime>
#include <random> // Include the random library

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

double calcdist(std::vector<City>& points) {
   
    double sum = 0;
    for (int i = 0; i < points.size()-1; i++) {
        double d = calculateDistance(points[i], points[i + 1]);
        sum = sum + d;
    }
    return sum;
}
void customSwap(std::vector<City>& cities, int i, int j) {
    std::swap(cities[i], cities[j]);
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML IMGUI", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Clock clock;
    bool show = false;

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

    for (int i = 0; i < cities.size(); i++) {
        sf::CircleShape circle(10);  // Create a circle with radius 50
        circle.setPosition(cities[i].x - 10, cities[i].y - 10);  // Set position to (100, 100)
        circle.setFillColor(sf::Color::White);  // Set fill color to red

        circles.push_back(circle);

    }

    sf::RectangleShape lineSegment;

    double recordDistance;
    std::vector<City> bestEver;
    double d = calcdist(cities);
    recordDistance = d;
    bestEver = cities;

    std::random_device rd; // Create a random device to seed the generator
    std::mt19937 generator(rd()); // Create the Mersenne Twister generator
    std::uniform_int_distribution<int> distribution(0, cities.size() - 1); // Create a uniform distribution for indices


    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart(); // get the time elapsed since the last restart and restart the clock
        float dt = elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
           
            if (event.type == sf::Event::Closed)
                window.close();
        }

       

        window.clear();
      
        // Drawing the background grid
        window.draw(grid);

       
        int i = distribution(generator);
        int j = distribution(generator);

        // Make sure i and j are distinct
        while (i == j) {
            j = distribution(generator);
        }
        customSwap(cities, i, j);
        d = calcdist(cities);


        if (d < recordDistance) {
            recordDistance = d;
            bestEver = cities;
            std::cout << " Record Distance is " << recordDistance << std::endl;
            std::cout.flush();

        }


        for (int i = 0; i < cities.size(); i++ ) {
            int nextIndex = (i + 1) % cities.size(); // Calculate the next city's index with wraparound
            lineSegment.setSize(sf::Vector2f(calculateDistance(cities[i], cities[nextIndex]), 1.5f));
            lineSegment.setFillColor(sf::Color::Green);
            lineSegment.setPosition(sf::Vector2f(cities[i].x, cities[i].y));
            lineSegment.setRotation(std::atan2(cities[nextIndex].y - cities[i].y, cities[nextIndex].x - cities[i].x) * 180.f / 3.14159265f);
            window.draw(lineSegment);
            window.draw(circles[i]);
        }

        for (int i = 0; i < bestEver.size(); i++) {
            int nextIndexx = (i + 1) % bestEver.size(); // Calculate the next city's index with wraparound
            lineSegment.setSize(sf::Vector2f(calculateDistance(bestEver[i], bestEver[nextIndexx]), 3.0f));
            lineSegment.setFillColor(sf::Color::Blue);
            lineSegment.setPosition(sf::Vector2f(bestEver[i].x, bestEver[i].y));
            lineSegment.setRotation(std::atan2(bestEver[nextIndexx].y - bestEver[i].y, bestEver[nextIndexx].x - bestEver[i].x) * 180.f / 3.14159265f);
            window.draw(lineSegment);

        }
      

        window.display();

    }

    return 0;
}
