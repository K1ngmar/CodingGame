
#include "Game.hpp"
#include <iostream>
#include "Entity.hpp"

const Position Game::base_positions[] = {
	{0,0},
	{17630, 9000}
};

const Position Game::center = {8815, 4500};

// construction

Game::Game() : round(0)
{
	int useless;

	std::cin >> base.x >> base.y; std::cin.ignore();
	std::cin >> useless; std::cin.ignore();

	if (base.x == 0)
		enemy_base = base_positions[1];
	else
		enemy_base = base_positions[0];
}

static Entity	readEntity(void)
{
	Entity entity;

	std::cin \
	>> entity.id \
	>> entity.type \
	>> entity.pos.x \
	>> entity.pos.y \
	>> entity.shield_life \
	>> entity.is_controlled \
	>> entity.health \
	>> entity.trajectory.x \
	>> entity.trajectory.y \
	>> entity.is_targeting \
	>> entity.target;
	entity.is_updated = false;
	std::cin.ignore();
	
	return entity;
}

void Game::setEntity(Entity& entity)
{
	switch(entity.type)
	{
		case HERO:
			heroes.push_back(entity); break ;
		case OPPONENT:
			opponents.push_back(entity); break ;
		case MONSTER:
			monsters.push_back(entity);
	}
}

void	Game::update(Entity& entity)
{
	vectity itr;

	switch(entity.type)
	{
		case HERO:
			break ;
		case OPPONENT:
			opponents.push_back(entity); break ;
		case MONSTER:
			monsters.push_back(entity);
	}
}

void	Game::init()
{
	int entity_count;

	std::cin >> health >> mana; std::cin.ignore();
	std::cin >> entity_count; std::cin.ignore();

	for (size_t i = 0; i < entity_count; ++i) {
		Entity entity = readEntity();
		if (round == 0)
			setEntity(entity);
		else
			update(entity);
	}
	++round;
}



//
