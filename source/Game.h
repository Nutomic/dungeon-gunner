/*
 * Game.h
 *
 *  Created on: 05.07.2012
 *      Author: Felix
 */

#ifndef DG_GAME_H_
#define DG_GAME_H_

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <Thor/Resources.hpp>

#include "World.h"
#include "sprites/TileManager.h"
#include "sprites/Player.h"
#include "types/String.h"
#include "util/Collection.h"
#include "util/Pathfinder.h"

class World;
class Player;
class Collection;

/*
 * Use vertex for tiles.
 */
class Game : private sf::NonCopyable {
// Public functions.
public:
	Game(sf::RenderWindow& window);
	~Game();

	void loop();

// Private functions.
private:
	void input();
	void render();
	void tick();

	void keyDown(const sf::Event& event);
	void keyUp(const sf::Event& event);
	void mouseUp(const sf::Event& event);

	void generate();
	String getFps();
	sf::Vector2<float> convertCoordinates(int x, int y);

// Private variables.
private:
	static const int FPS_GOAL;
	static const float TICKS_GOAL;

	World mWorld;

	sf::RenderWindow& mWindow;
	sf::Clock mClock;
	sf::View mView;
	//sf::Text mFps;

	Collection mCollection;
	TileManager mTileManager;
	Pathfinder mPathfinder;
	std::unique_ptr<Player> mPlayer;

	/// Milliseconds since the last tick.
	sf::Uint32 mElapsed;

	bool mQuit;
	bool mPaused;
};


#endif /* DG_GAME_H_ */
