#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics.hpp>

#include <vector>
#include "../header/guioverlay.h"

int Width = 1280;
int Height = 720;

const int NumRows = 14;
const int NumColumns = 25;
const float CellSize = 51.1f;

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

        Overlay::ShowSideBar();
        Overlay::ShowOverlay(&show);

        window.clear();

        // Drawing the background grid
        window.draw(grid);

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}