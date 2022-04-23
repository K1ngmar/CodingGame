
#include "Game.hpp"
#include "action.hpp"
#include <iostream>

#define DEBUG 1

#if DEBUG == 1
    #define TRACE() dprintf(2, "%s:%d\n", __FUNCTION__,  __LINE__)
#else
    #define TRACE()
#endif

static const Position default_hero_pos[] = {
	{5000, 2000},
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
				entity.is_attacker = heroes.size() == 1;
				entity.default_pos = default_hero_pos[heroes.size()];
				heroes.push_back(entity);
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
		std::cin >> entity.id \
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
		std::cin.ignore();
		entity.is_attacker = false;
		entity.default_pos = {center};

		this->setEntity(entity);
	}
	++round_nb;
}

////////////////
// Non member //
////////////////

bool		Game::isClosestHero(const Entity& hero, const Position& pos)
{
	Entity	closest;
	size_t	closest_dist = 420691337;

	for (vectity::iterator i = heroes.begin(); i != heroes.end(); ++i) {
		if (distance(i->pos, pos) < closest_dist) {
			closest = *i;
			closest_dist = distance(i->pos, pos);
		}
	}
	return (closest.id == hero.id);
}

Entity		Game::getClosestDangerousEntity(const Entity& cur)
{
	Entity	closest;
	bool	is_set = false;
	size_t	closest_dist = 420691337;

	for (vectity::iterator i = monsters.begin(); i != monsters.end(); ++i) {
		if (distance(base, i->pos) < closest_dist) {
			if (isClosestHero(cur, i->pos) == true || (i + 1 == monsters.end() && is_set == false)) {
				closest_dist = distance(base, i->pos);
				closest = *i;
				is_set = true;
			}
		}
	}
	return closest;
}

///////////
// Logic //
///////////

std::string	Game::defensiveStrat(const Entity& hero)
{
	if (monsters.size() > 0) {
		Entity danger = getClosestDangerousEntity(hero);
		if (hero.is_attacker == true && monsters.size() < 3)
			return hero.moveDefaultPos() + " no_threat";
		else if (danger.isCloseToPos(base) == true)
		{
			if (danger.isInWindRange(hero.pos) && mana > 10 && danger.shield_life == 0)
				return windy_day(center);
			else
				return move(danger.pos) + " next_pos?";
		}
		else {
			if (hero.is_attacker == false && distance(hero.pos, base) > 7500 && danger.movingToPos(base) == false)
				return hero.moveDefaultPos() + " bored";
			else
				return move(danger.pos) + " pos++;";
		}
	}
	else
		return hero.moveDefaultPos() + " default?";
}

std::string Game::generateAction(const Entity& hero)
{
	// if (base_health > enemy_health) {
		return this->defensiveStrat(hero);
	// }
	// else {
	// 	if (round_nb < 30)
	// 		this->farming_strat();
	// 	else if ()
	// }
}
