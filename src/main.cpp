#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iostream>
#include "../header/guioverlay.h"

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

double calculateTourDistance(const std::vector<City>& tour)
{
    double totalDistance = 0.0;
    for (std::size_t i = 0; i < tour.size() - 1; ++i)
    {
        totalDistance += calculateDistance(tour[i], tour[i + 1]);
    }
    totalDistance += calculateDistance(tour[tour.size() - 1], tour[0]);
    return totalDistance;
}

void drawCities(sf::RenderWindow& window, const std::vector<City>& cities)
{
    sf::CircleShape cityShape(5.f);
    cityShape.setFillColor(sf::Color::Red);

    for (const auto& city : cities)
    {
        cityShape.setPosition(city.x, city.y);
        window.draw(cityShape);
    }
}

void drawPath(sf::RenderWindow& window, const std::vector<City>& path)
{
    sf::VertexArray lines(sf::LineStrip);
    lines.setPrimitiveType(sf::LinesStrip);
    lines.resize(path.size());

    for (std::size_t i = 0; i < path.size(); ++i)
    {
        lines[i].position = sf::Vector2f(path[i].x, path[i].y);
        lines[i].color = sf::Color::Blue;
    }

    window.draw(lines);
}

std::vector<City> findShortestPath(const std::vector<City>& cities, sf::RenderWindow& window)
{
    std::vector<City> shortestPath;
    double shortestDistance = std::numeric_limits<double>::max();
    std::vector<City> currentPath = cities;

    do {
        double currentDistance = calculateTourDistance(currentPath);

        if (currentDistance < shortestDistance)
        {
            shortestDistance = currentDistance;
            shortestPath = currentPath;

            // Clear window and redraw the current best path
            window.clear(sf::Color::White);
            drawCities(window, cities);
            drawPath(window, shortestPath);
            window.display();
        }
    } while (std::next_permutation(currentPath.begin() + 1, currentPath.end(),
        [](const City& city1, const City& city2) {
            return std::less<int>()(city1.x, city2.x);
        }));

    return shortestPath;
}

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(Width, Height), "SFML IMGUI", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Clock clock;

    ImGui::SFML::Init(window);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    bool show = true;

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
      {100, 100},
      {200, 300},
      {400, 200},
      {600, 350},
      {700, 150}
    };
    std::vector<City> shortestPath = findShortestPath(cities, window);

    while (window.isOpen())
    {
        sf::Time elapsed = clock.restart(); // get the time elapsed since the last restart and restart the clock
        float dt = elapsed.asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ImGui::SFML::Update(window, clock.restart());

        window.clear();
        drawCities(window, cities);
        drawPath(window, shortestPath);
        // Drawing the background grid
        window.draw(grid);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
