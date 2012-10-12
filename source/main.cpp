/*
 * main.cpp
 *
 *  Created on: 19.07.2012
 *      Author: Felix
 */

#include "Game.h"
#include "util/Loader.h"
#include "util/Yaml.h"

/**
 * Creates Game object.
 */
int main(int argc, char* argv[]) {
	Yaml::setFolder("resources/yaml/");

	Loader::i().setFolder("resources/");
	Loader::i().setSubFolder<sf::Texture>("textures/");

	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Dungeon Gunner",
				sf::Style::Close | sf::Style::Titlebar);

    Game game(window);

	game.loop();

    return 0;
}
