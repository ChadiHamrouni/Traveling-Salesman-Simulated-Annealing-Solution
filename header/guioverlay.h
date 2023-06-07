#pragma once
#include <SFML/Graphics.hpp>

namespace Overlay {

	void ShowOverlay(bool* p_open);
	void ShowDockSpace(bool* p_open);
	void ShowSideBar();
	void SetupGrid();
	void RenderGrid(sf::RenderWindow& window);
}