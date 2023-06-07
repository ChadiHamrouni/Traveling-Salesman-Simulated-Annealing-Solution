#include <imgui.h>
#include <SFML/Graphics.hpp>
#include "../header/guioverlay.h"

const int NumRows = 14;
const int NumColumns = 25;
const float CellSize = 51.1f;
sf::Color green(13, 84, 93);

namespace Overlay {

    sf::VertexArray grid(sf::Lines);

    void ShowOverlay(bool* p_open)
    {
        static int location = 0;
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        if (location >= 0)
        {
            const float padding = 25.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;

            // Calculate the window size based on the percentage of the viewport size
            float window_width = work_size.x * 0.16f; // 20% of the viewport width
            float window_height = work_size.y * 0.16f; // 20% of the viewport height

            // Calculate the window position based on the percentage of the viewport size and the padding
            ImVec2 window_pos;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - window_width - padding) : (work_pos.x + work_size.x - window_width - padding);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - window_height - padding) : (work_pos.y + padding);

            // Set the next window size and position
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(ImVec2(window_width, window_height));

            ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        else if (location == -2)
        {
            // Center window
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            window_flags |= ImGuiWindowFlags_NoMove;
        }

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background

        if (ImGui::Begin("Physics Engine V0", p_open, window_flags))
        {
            int x = 420;
            std::string y = "Active Objects: " + std::to_string(x);
            ImGui::Text("Physics Engine V0");
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::Separator();

            ImGui::BulletText(y.c_str());
            ImGui::BulletText("Num Edges");
            ImGui::BulletText("Num Vertices");
            ImGui::BulletText("Collision OFF");
        }
        ImGui::End();
    }

    void ShowDockSpace(bool* p_open)
    {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;;

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", p_open, window_flags);

        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    void ShowSideBar() {

        const char s8_min = -128;
        const char u8_min = -128;
        const char s8_max = 250;
        const char u8_max = 300;

        static char   s8_v = 127;
        static ImU8   u8_v = 255;
        char s8_one = 1;
        const ImU8 u8_one = 1;

        ImGuiStyle& style = ImGui::GetStyle();

        style.Colors[ImGuiCol_TitleBgActive] = ImColor(4, 135, 156, 255); // Green
        style.Colors[ImGuiCol_TitleBg] = ImColor(4, 135, 156, 255);

        style.Colors[ImGuiCol_HeaderActive] = ImColor(4, 135, 156, 255);
        style.Colors[ImGuiCol_Header] = ImColor(4, 135, 156, 255);
        style.Colors[ImGuiCol_HeaderHovered] = ImColor(4, 135, 156, 255);

        style.Colors[ImGuiCol_TabHovered] = ImColor(4, 135, 156, 255);
        style.Colors[ImGuiCol_TabActive] = ImColor(4, 135, 156, 255);
        style.Colors[ImGuiCol_TabUnfocused] = ImColor(4, 135, 156, 255);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(4, 135, 156, 255);

        style.Colors[ImGuiCol_FrameBg] = ImColor(200, 10, 96, 255); // Pink
        style.Colors[ImGuiCol_FrameBgHovered] = ImColor(200, 10, 96, 255);

        style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255, 255);

        style.Colors[ImGuiCol_Button] = ImColor(200, 10, 96, 255);
        style.Colors[ImGuiCol_SliderGrab] = ImColor(4, 135, 156, 255);
        ImVec4 bulletColor(200 / 255.0f, 10 / 255.0f, 96 / 255.0f, 1.0f);

        static bool show_app_main_menu_bar = false;
        bool show_app_dockspace = true;

        if (show_app_dockspace)
            ShowDockSpace(&show_app_dockspace);

        static bool no_docking = false;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

        if (no_docking) window_flags |= ImGuiWindowFlags_NoDocking;

        const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

        ImGui::Begin("Settings");
        if (ImGui::CollapsingHeader("Help", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::SeparatorText("PHYSICS ENGINE V0:");
            std::vector<std::string> bulletTexts = {
                "Robot Dynamics AI Training",
                "Algorithms Visualization",
                "Natural Systems Simulations"
            };

            for (const auto& text : bulletTexts)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, bulletColor);
                ImGui::Bullet();
                ImGui::PopStyleColor();
                ImGui::SameLine();
                ImGui::Text(text.c_str());
            }

            ImGui::Dummy(ImVec2(0, 10)); // Add spacing after the bullets and text
        }

        if (ImGui::CollapsingHeader("Configuration", ImGuiTreeNodeFlags_DefaultOpen))
        {
            bool checked = true;
            static bool inputs_step = true;
            ImGuiIO& io = ImGui::GetIO();
            ImGui::Dummy(ImVec2(0, 5));

            if(ImGui::CollapsingHeader("Foces Settings", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::SeparatorText("General");
                ImGui::Checkbox("Gravity Force", &checked);
                ImGui::Checkbox("Friction Force", &checked);
                ImGui::Checkbox("Drag Force", &checked);

                ImGui::SeparatorText("Slides");
                ImGui::InputScalar("input s8", ImGuiDataType_S8, &s8_v, inputs_step ? &s8_one : NULL, NULL, "%d");
                ImGui::InputScalar("input u8", ImGuiDataType_U8, &u8_v, inputs_step ? &u8_one : NULL, NULL, "%u");

                ImGui::SliderScalar("slider s8", ImGuiDataType_S8, &s8_v, &s8_min, &s8_max, "%d");
                ImGui::SliderScalar("slider u8", ImGuiDataType_U8, &u8_v, &u8_min, &u8_max, "%u");

                ImGui::TreePop();
                ImGui::Spacing();
            }
            ImGui::Dummy(ImVec2(0, 5));
        }
        if (ImGui::CollapsingHeader("Input", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGuiIO& io = ImGui::GetIO();

            if (ImGui::IsMousePosValid())
                ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);

            else
                ImGui::Text("Mouse pos: <INVALID>");

            ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
            ImGui::Text("Mouse down:");
            for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
                if (ImGui::IsMouseDown(i)) {
                    ImGui::SameLine();
                    ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
                }
            ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);
        }
    }

    void SetupGrid() {
       
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

    }

    void RenderGrid(sf::RenderWindow& window) {
        window.draw(grid);
    }
}