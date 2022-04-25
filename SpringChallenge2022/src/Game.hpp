
#pragma once

#include "Position.hpp"
#include <vector>

struct Game
{
// statitcs

	static const Position base_positions[];
	static const Position center;

// typedefs

	typedef std::vector<Entity> vectity;

// variables

	Position	base;
	Position	enemy_base;
	int			health;
	int			enemy_health;
	int			mana;
	int			enemy_mana;
	int			round;

	vectity		heroes;
	vectity		monsters;
	vectity		opponents;

// construct

	Game();

	void	setEntity(Entity& entity);
	void	update(Entity& entity);
	void	init();

};
