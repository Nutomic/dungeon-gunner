/*
 * main.cpp
 *
 *  Created on: 19.07.2012
 *      Author: Felix
 */

#include "Game.h"
#include "util/Loader.h"
#include "util/Yaml.h"
#include "util/Log.h"

#include "util/Vector.h"

// TODO: remove right click move (also in readme)

/**
 * Creates Game object.
 */
int main(int argc, char* argv[]) {
	Yaml::setFolder("res/yaml/");
	Loader::i().setFolder("res/");
	Loader::i().setSubFolder<sf::Texture>("textures/");

	Yaml windowConfig("window.yaml");
	sf::VideoMode mode(windowConfig.get("resolution_width", 800),
	                   windowConfig.get("resolution_height", 600),
	                   32);
	sf::Uint32 style;
	if (windowConfig.get("fullscreen", false))
		style = sf::Style::Fullscreen;
	else if (windowConfig.get("borderless", false))
		style = sf::Style::None;
	else
		style = sf::Style::Titlebar;
    tgui::Window window(mode, "Dungeon Gunner", style);

    Vector2f::SCREEN_HEIGHT = window.getSize().y;

	if (!window.globalFont.loadFromFile("res/DejaVuSans.ttf"))
		LOG_W("Failed to load font at 'res/DejaVuSans.ttf'");

    Game game(window);

	game.loop();

    return 0;
}

