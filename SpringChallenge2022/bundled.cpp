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

static const Position base_positions[] = {
	{0,0},
	{17630, 9000}
};

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
	Target		target;

/////////////
// Helpful //
/////////////
public:

	bool	isCloseToPos(const Position& pos) const;
	bool	isInSpellRange(const Position& pos) const;
	bool	isInWindRange(const Position& pos) const;
	
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

//////////
// Game //
//////////
class Game
{
	//////////////
	// Typedefs //
	//////////////
	public:

		typedef std::vector<Entity> vectity;

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
		vectity		monsters;
		vectity		opponents;
		size_t		round_nb;

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

		void clearEntities();
		void setEntity(Entity& entity);

	public:

		void parseRound();

	/////////////
	// Helpers //
	/////////////
	public:

		bool	isClosestHero(const Entity& hero, const Position& pos);

		Entity	getClosestDangerousEntity(const Entity& hero);

	///////////
	// Logic //
	///////////
	public:

		std::string generateAction(const Entity& hero);

		std::string	defensiveStrat(const Entity& hero);

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
	return "SPELL WIND " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " ITS WINDY OUT HERE";
}

std::string	shield(const int id) {
	return "SPELL SHIELD " + std::to_string(id) + " PROTECC";
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
	return (distance(pos, this->pos) <= 6000);
}

bool	Entity::isInSpellRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 2200);
}

bool	Entity::isInWindRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 1280);
}

Position	Entity::nextPos() const {
	Position ret;
	std::cerr << "x: " << this->trajectory.x << ", y: " << this->trajectory.y << std::endl;
	ret.x = this->pos.x + this->trajectory.x;
	ret.y = this->pos.y + this->trajectory.y;
	return ret;
}

std::string	Entity::moveDefaultPos() const
{
	if (this->is_attacker == true)
		return move(center);
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

static const Position default_hero_pos[] = {
	{5000, 2000},
	{3500, 3500},
	{1500, 5000},
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
	Entity closest;
	size_t closest_dist = 420691337;

	for (vectity::iterator i = monsters.begin(); i != monsters.end(); ++i) {
		if (this->isClosestHero(cur, i->pos)) {
			if (distance(base, i->pos) < closest_dist) {
				closest_dist = distance(base, i->pos);
				closest = *i;
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
		if (danger.isCloseToPos(base) == true) {
			// use wind
			if (danger.isInWindRange(hero.pos) && mana > 10 && danger.shield_life == 0)
				return windy_day(center);
			else
				return move(danger.nextPos()) + " next_pos?";
		}
		else
			return move(danger.nextPos()) + " pos++;";
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
		return (abs(h.x - e.x) + abs(h.y - e.y));
	}

/*-- File: src/Position.cpp end --*/
