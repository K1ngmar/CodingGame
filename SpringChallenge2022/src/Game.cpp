
#include "Game.hpp"
#include <iostream>
#include <algorithm>
#include "Entity.hpp"

const Position Game::base_positions[] = {
	{0,0},
	{17630, 9000}
};

const Position Game::center = {8815, 4500};

static const Position* default_pos;

static const Position default_left[] = {
	{6500, 1700},
	{2000, 6000},
	{11000, 5000}
};

static const Position default_right[] = {
	{11000, 7000},
	{13500, 4000},
	{6000, 4000}
};

// construction

Game::Game() : round(0), defender_amt(HERO_AMT)
{
	int useless;

	std::cin >> base.x >> base.y; std::cin.ignore();
	std::cin >> useless; std::cin.ignore();

	if (base.x == 0) {
		enemy_base = base_positions[1];
		default_pos = default_left;
	}
	else {
		enemy_base = base_positions[0];
		default_pos = default_right;
	}
}

Entity	Game::readEntity()
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
	std::cin.ignore();
	entity.my_base = base;
	entity.default_pos = center;
	entity.current_target = NULL;

	return entity;
}

void Game::setEntity(Entity& entity)
{
	switch(entity.type)
	{
		case HERO:
			entity.default_pos = default_pos[heroes.size()];
			entity.is_attacker = heroes.size() == 2;
			heroes.push_back(entity); break ;
		case OPPONENT:
			opponents.push_back(entity); break ;
		case MONSTER:
			monsters.push_back(entity);
	}
}

void	Game::update(Entity& entity)
{
	vectity::iterator itr;

	switch(entity.type)
	{
		case MONSTER:
			itr = std::find(monsters.begin(), monsters.end(), entity);
			if (itr != monsters.end()) {
				itr->pos = itr->nextPos();
				if (itr->pos.x < base_positions[0].x || itr->pos.x > base_positions[1].x ||
					itr->pos.y < base_positions[0].y || itr->pos.y > base_positions[1].y)
						monsters.erase(itr);
			}
			else
				setEntity(entity);
			break ;
		default:
			setEntity(entity);
	}
}

void	Game::clearEntities()
{
	heroes.clear();
	opponents.clear();
	active_targets.clear();
	monsters.clear();
}

void	Game::parseRound()
{
	int entity_count;

	std::cin >> health >> mana; std::cin.ignore();
	std::cin >> enemy_health >> enemy_mana; std::cin.ignore();
	std::cin >> entity_count; std::cin.ignore();

	this->clearEntities();
	for (size_t i = 0; i < entity_count; ++i) {
		Entity entity = readEntity();

		setEntity(entity);
		if (controlled.find(entity.id) != controlled.end()) {
			if (entity.movingToPos(base))
				controlled.erase(entity.id);
		}
	}
	std::sort(monsters.begin(), monsters.end());
	++round;
	if (round > 100)
		defender_amt = 2;
}

///////////
// Logic //
///////////

struct permutePair {
	Entity* hero;
	Entity* target;
};

void	Game::permutationNation()
{
	size_t		best_rating = 420691337;
	size_t		rating;
	permutePair tmp[HERO_AMT];
	permutePair	best[HERO_AMT];
	Entity*		monster;

	for (size_t offset = 0; offset < defender_amt; offset++) {
		for (size_t j = 1; j < defender_amt; j++) {
			rating = 0;
			for (size_t i = 0; i < defender_amt; i++) {
				if ((i * j) % defender_amt > monsters.size())
					monster = NULL;
				else
					monster = &(monsters[(i * j) % defender_amt]);
				tmp[i] = {
						&(heroes[(i + offset) % defender_amt]),
						monster
					};
				if (monster)
					rating += distance(tmp[i].hero->pos, monster->pos);
			}
			if (rating < best_rating) {
				for (size_t i = 0; i < defender_amt; i++)
					best[i] = {tmp[i].hero, tmp[i].target};
			}
		}
	}
	for (size_t i = 0; i < defender_amt; i++) {
		best[i].hero->current_target = best[i].target;
	}
}

bool	Game::attackTarget(const Entity& hero)
{
	if (hero.current_target) 
		return move(hero.current_target->pos, " PERM");
	return false;
}

bool	Game::useSpell(const Entity& hero)
{
	Entity* target = hero.current_target;

	if (target && mana >= 10 && target->shield_life == 0) {
		if (target->isInRange6000(base) && target->isInRange5000(base) == false
			&& target->isInSpellRange(hero.pos) && target->movingToPos(base)
			&& controlled.find(target->id) != controlled.end())
				return control(target->id, enemy_base, " NOPE");
		if (target->isInRange4500(base) && target->isInWindRange(hero.pos) && target->health > 10)
			return windy_day(center, " WOOSH");
	}
	return false;
}

bool	Game::attack(const Entity& hero)
{
	if (defender_amt < 3 && hero.is_attacker) {
		if (mana > 50) {
			// do some attack stuff
		}	
	}
	return false;
}

bool	Game::annoyingEnemy(const Entity& hero)
{
	if (mana >= 10) {
		for (const Entity& enemy: opponents) {
			if (enemy.isInRange7500(base) && round > 80) {
				if (enemy.isInSpellRange(hero.pos) && hero.shield_life == 0)
					return shield(hero.id, " NOPE");
				else if (enemy.isInSpellRange(hero.pos))
					return windy_day(center, " BYE");
			}
		}
	}
	return false;
}

bool	Game::defend(Entity& hero)
{
	bool attacked = false;

	attacked = attacked || attack(hero);
	attacked = attacked || annoyingEnemy(hero);
	attacked = attacked || useSpell(hero);
	attacked = attacked || attackTarget(hero);
	return attacked;
}

bool	Game::moveDefaultPos(Entity& hero)
{
	int				radius = 1600;
	Position		middle = hero.default_pos;
	Position		goal;
	static float 	tatta = 0;
	tatta += 0.420; // tatta + dtatta

	goal.x = radius * cos(tatta) + middle.x;
	goal.y = radius * sin(tatta) + middle.y;
	move(goal, " IDLE");
	return true;
}

void Game::performAction(Entity& hero)
{
	bool action = false;
	if (monsters.size() > 0)
		permutationNation();

	action = action || defend(hero);
	action = action || moveDefaultPos(hero);
}

/////////////
// Actions //
/////////////

bool	Game::move(const Position& pos, const std::string& addon) {
	std::cout << "MOVE " << pos.x << " " << pos.y << addon << std::endl;
	return true;
}

bool	Game::windy_day(const Position& pos, const std::string& addon) {
	mana -= 10;
	std::cout << "SPELL WIND " << pos.x << " " << pos.y << addon << std::endl;
	return true;
}

bool	Game::shield(const int id, const std::string& addon) {
	mana -= 10;
	std::cout << "SPELL SHIELD " << id << addon << std::endl;
	return true;
}

bool	Game::control(const int id, const Position& pos, const std::string& addon) {
	mana -= 10;
	this->controlled.insert(id);
	int y_offset = (rand() % 5000) * (rand() % 2) ? 1 : -1;
	std::cout << "SPELL CONTROL " << id << " " << pos.x << " " << pos.y + y_offset << addon << std::endl;
	return true;
}
