
#pragma once

#include "Position.hpp"
#include "Entity.hpp"
#include <vector>

# define HERO_AMT 3

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
	vectity		active_targets;

// construct

	Game();

	Entity	readEntity();
	void	setEntity(Entity& entity);
	void	update(Entity& entity);
	void	parseRound();
	void	clearEntities();

// logic

	bool	stoopidAttack(Entity& hero);
	void	permutationNation();
	bool	attackTarget(const Entity& hero);

	bool	defend(Entity& hero);
	bool	moveDefaultPos(Entity& hero);

	void	performAction(Entity& hero);

// actions

	bool	move(const Position& pos, const std::string& addon = "");
	bool	windy_day(const Position& pos, const std::string& addon = "");
	bool	shield(const int id, const std::string& addon = "");
	bool	control(const int id, const Position& pos, const std::string& addon = " CONTROLLAA");

};
