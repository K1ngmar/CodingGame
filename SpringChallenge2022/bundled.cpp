/*-- File: src/main.cpp start --*/

/*-- #include "src/Game.hpp" start --*/


/*-- #include "src/Position.hpp" start --*/


#include <math.h>

//////////////
// Position //
//////////////

struct Position
{
	int x;
	int y;
};

///////////////
// Operators //
///////////////

	bool operator == (const Position& lhs, const Position& rhs);

////////////////
// Funky bois //
////////////////

	size_t		distance(const Position& h, const Position& e);

/*-- #include "src/Position.hpp" end --*/
/*-- #include "src/Entity.hpp" start --*/


/*-- #include "src/Position.hpp" start --*/
/*-- #include "src/Position.hpp" end --*/
#include <istream>
#include <map>

static const Position center = {8900, 4500};

struct Entity
{
///////////
// Types //
///////////

	enum Type {
		MONSTER,
		HERO,
		OPPONENT
	};

	enum Target {
		NONE,
		BASE,
		ENEMY
	};

	typedef std::map<size_t, Entity>	dist_map;

///////////////
// Variables //
///////////////

	int			id;
	bool		is_attacker;
	Position	default_pos;
	Type		type;
	Position	pos;
	int 		shield_life;
	int 		is_controlled;
	int			health;
	Position	trajectory;
	int			is_targeting;
	Target		targeting;
	dist_map	targets;

/////////////
// Helpful //
/////////////
public:

	bool	isCloseToPos(const Position& pos) const;
	bool	isInRange6000(const Position& pos) const;
	bool	isInRange7500(const Position& pos) const;


	bool	isInSpellRange(const Position& pos) const;
	bool	isInWindRange(const Position& pos) const;

	bool	movingToPos(const Position& pos) const;

	Position	nextPos() const;

	std::string	moveDefaultPos() const;

}; /* end of Entity class */

///////////////
// Operators //
///////////////

	std::istream& operator >> (std::istream& is, const Entity::Type& type);
	std::istream& operator >> (std::istream& is, const Entity::Target& target);

/*-- #include "src/Entity.hpp" end --*/
#include <vector>
#include <set>

//////////
// Game //
//////////
class Game
{
	//////////////
	// Typedefs //
	//////////////
	public:

		typedef std::vector<Entity>			vectity;
		typedef std::set<int>				target_map;
		typedef std::map<size_t, Entity>	dm;

	//////////////////////
	// Member variables //
	//////////////////////
	public:

		Position	base;
		int			base_health;
		Position	enemy_base;
		int			enemy_health;
		int 		mana;
		int			enemy_mana;
		vectity		heroes;
		vectity		opponents;
		dm			monsters;
		size_t		round_nb;
		target_map	active_targets;
		

	//////////////////
	// Construction //
	//////////////////
	private:

		Game(const Game& x);
		Game& operator = (const Game& x);

	public:

		Game();

	///////////////////
	// Funky Members //
	///////////////////
	private:

		void	clearEntities();
		void	setEntity(Entity& entity);

	public:

		void	createDistMap();
		void	parseRound();

	/////////////
	// Helpers //
	/////////////
	public:

		// bool isClosestTarget(const Entity& hero);
		bool isClosestHero(int id, Position& pos) const;
		bool annoyingEnemy(const Entity& target) const;
		Entity getAnnoyingEnemy(const Entity& target) const;

	///////////
	// Logic //
	///////////
	public:

		Entity		getBestDefendingTarget(const Entity& hero);
		std::string generateAction(const Entity& hero);

	/////////////////
	/// Strategies //
	/////////////////
	public:

		std::string	defensiveStrat(const Entity& hero);
		std::string	attackingStrat(const Entity& hero);

};

/*-- #include "src/Game.hpp" end --*/
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

/*-- File: src/main.cpp end --*/
/*-- File: src/Action.cpp start --*/

/*-- #include "src/Action.hpp" start --*/


/*-- #include "src/Position.hpp" start --*/
/*-- #include "src/Position.hpp" end --*/
#include <string>

	std::string	move(const Position& pos);
	std::string	windy_day(const Position& pos);
	std::string	shield(const int id);
	std::string	control(const int id, const Position& pos);

/*-- #include "src/Action.hpp" end --*/

std::string	move(const Position& pos) {
	return "MOVE " + std::to_string(pos.x) + " " + std::to_string(pos.y);
}

std::string	windy_day(const Position& pos) {
	return "SPELL WIND " + std::to_string(pos.x) + " " + std::to_string(pos.y);
}

std::string	shield(const int id) {
	return "SPELL SHIELD " + std::to_string(id);
}

std::string	control(const int id, const Position& pos) {
	return "SPELL CONTROL " + std::to_string(id) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " CONTROLAAAA";
}

/*-- File: src/Action.cpp end --*/
/*-- File: src/Entity.cpp start --*/

/*-- #include "src/Entity.hpp" start --*/
/*-- #include "src/Entity.hpp" end --*/
/*-- #include "src/Action.hpp" start --*/
/*-- #include "src/Action.hpp" end --*/
#include <iostream>

///////////////
// Operators //
///////////////

std::istream& operator >> (std::istream& is, Entity::Type& type) {
	int _type;
	is >> _type;
	type = static_cast<Entity::Type>(_type);
	return is;
}

std::istream& operator >> (std::istream& is, Entity::Target& target) {
	int _target;
	is >> _target;
	target = static_cast<Entity::Target>(_target);
	return is;
}

/////////////
// Helpful //
/////////////

bool	Entity::isCloseToPos(const Position& pos) const {
	return (distance(pos, this->pos) <= 4000);
}

bool	Entity::isInRange6000(const Position& pos) const {
	return distance(pos, this->pos) <= 6000;
}

bool	Entity::isInRange7500(const Position& pos) const {
	return distance(pos, this->pos) <= 7500;
}

bool	Entity::isInSpellRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 2200);
}

bool	Entity::isInWindRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 1280);
}

bool	Entity::movingToPos(const Position& pos) const {
	return distance(this->nextPos(), pos) <  distance(this->pos, pos);
}

Position	Entity::nextPos() const {
	Position ret;
	ret.x = this->pos.x + this->trajectory.x;
	ret.y = this->pos.y + this->trajectory.y;
	return ret;
}

std::string	Entity::moveDefaultPos() const
{
	// if (this->is_attacker == true)
	// 	return move(center);
	return move(this->default_pos);
}

/*-- File: src/Entity.cpp end --*/
/*-- File: src/Game.cpp start --*/

/*-- #include "src/Game.hpp" start --*/
/*-- #include "src/Game.hpp" end --*/
/*-- #include "src/action.hpp" start --*/


/*-- #include "src/Position.hpp" start --*/
/*-- #include "src/Position.hpp" end --*/
#include <string>

	std::string	move(const Position& pos);
	std::string	windy_day(const Position& pos);
	std::string	shield(const int id);
	std::string	control(const int id, const Position& pos);

/*-- #include "src/action.hpp" end --*/
#include <iostream>

#define DEBUG 1

#if DEBUG == 1
    #define TRACE() dprintf(2, "%s:%d\n", __FUNCTION__,  __LINE__)
#else
    #define TRACE()
#endif

static const Position *default_hero_pos;
	
static const Position default_left_pos[3] = {
	{6000, 1500},
	{3000, 5000},
	{12000, 5000}, // attacker
};

static const Position default_right_pos[3] = {
	{15000, 3000},
	{11500, 6500},
	{5500, 3500}, // attacker
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
				entity.is_attacker = heroes.size() == 2;
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
		++itr;
	}
	return hero.targets.begin()->second;
}

////////////////
// Strategies //
////////////////

std::string	Game::defensiveStrat(const Entity& hero)
{
	Entity target = getBestDefendingTarget(hero);

	if (monsters.size() > 0) {
		if (target.isInRange7500(base) == true)
			return move(target.pos);
	}
	return move(hero.default_pos);
}

std::string Game::attackingStrat(const Entity& hero)
{
	return move(hero.default_pos);
}

std::string Game::generateAction(const Entity& hero)
{
	if (hero.is_attacker == true)
		return attackingStrat(hero);
	return defensiveStrat(hero);
}

/*-- File: src/Game.cpp end --*/
/*-- File: src/Position.cpp start --*/

/*-- #include "src/Position.hpp" start --*/
/*-- #include "src/Position.hpp" end --*/

///////////////
// Operators //
///////////////

	bool operator == (const Position& lhs, const Position& rhs) {
		return (lhs.x == rhs.x && lhs.y == rhs.y);
	}

////////////////
// Funky bois //
////////////////

	size_t		distance(const Position& h, const Position& e) {
		//((x1 - x2)^2 + (y1 - y2)^2)^0.5
		return ceil(pow((pow(h.x - e.x, 2) + pow(h.y - e.y, 2)), 0.5));
	}

/*-- File: src/Position.cpp end --*/
