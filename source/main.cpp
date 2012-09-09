/*
 * main.cpp
 *
 *  Created on: 19.07.2012
 *      Author: Felix
 */

#include "Game.h"
#include "util/Loader.h"

/**
 * Creates Game object.
 */
int main(int argc, char* argv[]) {
	Loader::i().setFolder("resources/");
	Loader::i().setSubFolder<sf::Texture>("textures/");

    Game game(Vector2i(800, 600));

	game.loop();

    return 0;
}
