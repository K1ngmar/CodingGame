
#include "Game.hpp"
#include "action.hpp"
#include <iostream>

#define DEBUG 1

#if DEBUG == 1
    #define TRACE() dprintf(2, "%s:%d\n", __FUNCTION__,  __LINE__)
#else
    #define TRACE()
#endif

static const Position *default_hero_pos;
	
static const Position default_left_pos[3] = {
	{5000, 1000},
	{3500, 3000},
	{1500, 5000},
};

static const Position default_right_pos[3] = {
	{16000, 4000},
	{14000, 5500},
	{13000, 8000},
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
	if (base == base_positions[0]) {
		default_hero_pos = default_left_pos;
		enemy_base = base_positions[1];
	}
	else {
		default_hero_pos = default_right_pos;
		enemy_base = base_positions[0];
	}
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
		active_targets.clear();
	}

	void Game::setEntity(Entity& entity)
	{
		switch(entity.type)
		{
			case Entity::MONSTER:
				monsters[distance(base, entity.pos)] = entity; break;
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

void Game::createDistMap()
{
	for (Entity& hero: heroes)
		for (auto monster: monsters)
			hero.targets[distance(hero.pos, monster.second.pos)] = monster.second;
}

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
		>> entity.targeting;
		std::cin.ignore();
		entity.is_attacker = false;
		entity.default_pos = {center};

		this->setEntity(entity);
	}
	createDistMap();
	++round_nb;
}

/////////////
// Helpers //
/////////////

bool	Game::isClosestHero(int hero_id, Position& pos) const
{
	size_t	dist = 420691337;
	int		id = hero_id;

	for (const Entity& hero: heroes) {
		if (distance(hero.pos, pos) < dist) {
			id = hero.id;
			dist = distance(hero.pos, pos);
		}
	}
	return (hero_id == id);
}

bool Game::annoyingEnemy(const Entity& target) const
{
	for (const Entity& enemy: opponents) {
		if (target.isInSpellRange(enemy.pos) == true || target.isInSpellRange(enemy.nextPos()) == true) {
			return true;
		}
	}
	return false;
}

Entity Game::getAnnoyingEnemy(const Entity& target) const
{
	for (const Entity& enemy: opponents) {
		if (target.isInSpellRange(enemy.pos) == true || target.isInSpellRange(enemy.nextPos()) == true) {
			return enemy;
		}
	}
	std::cerr << "this should not happen" << std::endl;
	return target;
}

///////////
// Logic //
///////////

Entity Game::getBestDefendingTarget(const Entity& hero)
{
	dm::iterator itr = monsters.begin();

	while (itr != monsters.end() && itr->second.targeting == Entity::BASE) {
		if ((active_targets.find(itr->second.id) == active_targets.end())) {
			if (isClosestHero(hero.id, itr->second.pos) == true || (itr->second.isCloseToPos(base) && itr->second.shield_life > 0))
				return (itr->second);
		}
		if (distance(itr->second.pos, hero.pos) < 2000 && distance(itr->second.pos, base) < 4500)
			return (itr->second);
		++itr;
	}
	return hero.targets.begin()->second;
}

////////////////
// Strategies //
////////////////

std::string	Game::defensiveStrat(const Entity& hero)
{
	if (annoyingEnemy(hero) == true && mana > 10) {
		Entity enemy = getAnnoyingEnemy(hero);

		if (distance(enemy.nextPos(), enemy.pos) < 800)
			return control(enemy.id, enemy_base);
		if (enemy.shield_life == 0)
			return windy_day(center);
		else if (hero.shield_life == 0)
			return shield(hero.id);
	}
	// if an enemy is attacking but is not in range
	if (hero.is_attacker == true && opponents.size() > 0 && hero.isInSpellRange(opponents[0].pos) == false)
		return move(opponents[0].pos);
	if (monsters.size() > 0) {
		Entity target = getBestDefendingTarget(hero);

		if (monsters.size() > 2 || active_targets.find(target.id) == active_targets.end()) {
			active_targets.insert(target.id);
			if (distance(target.pos, base) < 4500 && target.shield_life == 0 && mana > 10)
				return windy_day(center);
			if (target.shield_life == 0 && target.isCloseToPos(base) && target.isInWindRange(hero.pos) && mana > 10 && round_nb > 40)
				return windy_day(center);
			if (target.movingToPos(base) == true)
				return move(target.pos);
		}
	}
	return hero.moveDefaultPos();
}

std::string Game::attackingStrat(const Entity& hero)
{

}

std::string Game::generateAction(const Entity& hero)
{
	return this->defensiveStrat(hero);
}
