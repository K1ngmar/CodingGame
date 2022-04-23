
#include "Game.hpp"
#include <iostream>

int main()
{
	Game		game;
	std::string action;

	while(true)
	{
		game.parseRound();
		for (const Entity& hero: game.heroes) {
			action = game.generateAction(hero);
			std::cout << action << std::endl;
		}
	}
	return (0);
}
