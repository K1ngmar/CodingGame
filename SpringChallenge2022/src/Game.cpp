
#include "Game.hpp"
#include "action.hpp"
#include <iostream>

const Position Game::default_hero_pos[] = {
	{5000, 800},
	{3500, 3500},
	{1500, 5000},
};

static const Position base_positions[] = {
	{0,0},
	{17630, 9000}
};


//////////////////
// Construction //
//////////////////

Game::Game(): round_nb(0)
{
	int dont_need_this_but_has_to_be_read_otherwise_things_will_go_wrong;
	std::cin >> base.x >> base.y; std::cin.ignore();
	if (base == base_positions[0])
		enemy_base = base_positions[1];
	else
		enemy_base = base_positions[0];
	std::cin >> dont_need_this_but_has_to_be_read_otherwise_things_will_go_wrong; std::cin.ignore();
}

///////////////////
// Funky headers //
///////////////////

// private

	void Game::clearEntities()
	{
		heroes.clear();
		monsters.clear();
		opponents.clear();
	}

	void Game::setEntity(Entity& entity)
	{
		switch(entity.type)
		{
			case Entity::MONSTER:
				monsters.push_back(entity); break;
			case Entity::HERO:
				heroes.push_back(entity);
				heroes[heroes.size() - 1].default_pos = default_hero_pos[heroes.size() - 1];
				break;
			case Entity::OPPONENT:
				opponents.push_back(entity); break;
		}
	}

// public

void Game::parseRound()
{
	int		entity_count;
	
	std::cin >> base_health >> mana; std::cin.ignore();
	std::cin >> enemy_health >> enemy_mana; std::cin.ignore();
		
	std::cin >> entity_count; std::cin.ignore();

	this->clearEntities();
	for (int i = 0; i < entity_count; ++i)
	{
		Entity	entity;

		this->setEntity(entity);
	}
}

///////////
// Logic //
///////////

	std::string Game::generateAction(const Entity& hero) {
		return move(hero.default_pos);
	}
