
#include "Game.hpp"
#include <iostream>

int main()
{
	Game		game;

	while(true)
	{
		game.parseRound();
		for (Entity& hero: game.heroes) {
			game.performAction(hero);
		}
	}
	return (0);
}
