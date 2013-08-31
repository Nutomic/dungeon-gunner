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

/**
 * Creates Game object.
 */
int main(int argc, char* argv[]) {
	Yaml::setFolder("res/yaml/");

	Loader::i().setFolder("res/");
	Loader::i().setSubFolder<sf::Texture>("textures/");

    tgui::Window window(sf::VideoMode(1024, 768, 32), "Dungeon Gunner",
				sf::Style::Close | sf::Style::Titlebar);

	if (!window.globalFont.loadFromFile("res/DejaVuSans.ttf"))
		LOG_W("Failed to load font at 'res/DejaVuSans.ttf'");

    Game game(window);

	game.loop();

    return 0;
}

