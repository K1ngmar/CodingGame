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

	typedef std::map<size_t, Entity> threat_map;

///////////////
// Variables //
///////////////

	int			id;
	Type		type;
	Position	pos;
	int 		shield_life;
	int 		is_controlled;
	int			health;
	Position	trajectory;
	int			near_base;
	Target		targeting;

	Position	default_pos;
	threat_map	threats;

	bool		is_attacker;
	Entity*		current_target;

/////////////
// Helpful //
/////////////
public:

	bool	isInRange5000(const Position& pos) const;
	bool	isInRange6000(const Position& pos) const;
	bool	isInRange7500(const Position& pos) const;

	bool	isInSpellRange(const Position& pos) const;
	bool	isInWindRange(const Position& pos) const;

	bool	movingToPos(const Position& pos) const;

	Position	nextPos() const;

	Position	idle() const;

}; /* end of Entity class */

///////////////
// Operators //
///////////////

	bool operator == (const Entity& lhs, const Entity& rhs);

	std::istream& operator >> (std::istream& is, const Entity::Type& type);
	std::istream& operator >> (std::istream& is, const Entity::Target& target);

/*-- #include "src/Entity.hpp" end --*/
#include <vector>
#include <set>

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

//////////
// Game //
//////////
class Game
{
	typedef std::vector<Entity>			evec;
	typedef std::map<size_t, Entity>	threat_map;
	typedef std::set<int>				atarget;	

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
		int			round;

		evec		heroes;
		evec		enemies;
		threat_map	monsters;
		atarget		active_targets;
		

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

		void	setEntity(Entity& entity);
		void	createDangerMap(Entity& entity);

	public:

		void	parseRound();

	/////////////
	// Helpers //
	/////////////
	public:

		bool isClosestHero(int id, const Position& pos) const;
		bool isClosestHero(const Entity& hero, const Entity& entity) const;
		bool isActiveTarget(const Entity& monster) const;

	///////////
	// Logic //
	///////////
	public:

		std::string generateAction(const Entity& hero);

	/////////////////
	/// Strategies //
	/////////////////
	public:

		std::string	defensiveStrat(const Entity& hero);
		std::string	attackingStrat(const Entity& hero);

	//////////////
	// Actiones //
	//////////////
	public:

		std::string	move(const Position& pos);
		std::string	windy_day(const Position& pos);

		std::string	shield(const int id);

		std::string	control(const int id, const Position& pos = center);

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
/*-- File: src/Entity.cpp start --*/

/*-- #include "src/Entity.hpp" start --*/
/*-- #include "src/Entity.hpp" end --*/
#include <iostream>

///////////////
// Operators //
///////////////

bool operator == (const Entity& lhs, const Entity& rhs) {
	return lhs.id == rhs.id;
}

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

bool	Entity::isInRange5000(const Position& pos) const {
	return (distance(pos, this->pos) <= 5000);
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

Position	Entity::idle() const {
	if (distance(pos, default_pos) > 800)
		return default_pos;
	int x = 800 * ((rand() % 2) ? 1 : -1);
	int y = 800 * ((rand() % 2) ? 1 : -1);
	return {pos.x + x, pos.y + y};
}

/*-- File: src/Entity.cpp end --*/
/*-- File: src/Game.cpp start --*/

/*-- #include "src/Game.hpp" start --*/
/*-- #include "src/Game.hpp" end --*/
#include <iostream>

#define DEBUG 1

#if DEBUG == 1
    #define TRACE() dprintf(2, "%s:%d\n", __FUNCTION__,  __LINE__)
#else
    #define TRACE()
#endif

//////////////////
// Construction //
//////////////////

Game::Game(): round(0)
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

	void Game::setEntity(Entity& entity)
	{
		switch(entity.type)
		{
			case Entity::MONSTER:
				monsters[distance(entity.pos, base)] = entity;
				break ;
			case Entity::HERO:
				entity.default_pos = default_hero_pos[heroes.size()];
				heroes.push_back(entity);
				break ;
			case Entity::OPPONENT:
				enemies.push_back(entity);
				break ;
		}
	}

void	Game::createDangerMap(Entity& hero)
{
	for (auto monster: monsters)
		hero.threats[distance(monster.second.pos, hero.pos)] = monster.second;
}

// public

void Game::parseRound()
{
	int		entity_count;
	
	std::cin >> base_health >> mana; std::cin.ignore();
	std::cin >> enemy_health >> enemy_mana; std::cin.ignore();
		
	std::cin >> entity_count; std::cin.ignore();

	monsters.clear();
	heroes.clear();
	enemies.clear();
	active_targets.clear();
	for (int i = 0; i < entity_count; ++i)
	{
		Entity	entity;
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
		>> entity.near_base \
		>> entity.targeting;
		std::cin.ignore();
		entity.is_attacker = false;
		entity.default_pos = {center};

		this->setEntity(entity);
	}
	for (Entity& hero: heroes)
		createDangerMap(hero);
	++round;
}

/////////////
// Helpers //
/////////////

bool	Game::isClosestHero(int hero_id, const Position& pos) const
{
	size_t	dist = 420691337;
	int		id;

	for (const Entity& hero: heroes) {
		if (distance(hero.pos, pos) < dist) {
			id = hero.id;
			dist = distance(hero.pos, pos);
		}
	}
	return (hero_id == id);
}

bool	Game::isClosestHero(const Entity& hero, const Entity& entity) const
{
	return isClosestHero(hero.id, entity.pos);
}

bool	Game::isActiveTarget(const Entity& monster) const
{
	return active_targets.find(monster.id) != active_targets.end();
}

///////////
// Logic //
///////////



////////////////
// Strategies //
////////////////

std::string	Game::defensiveStrat(const Entity& hero)
{
	if (monsters.size() > 0 && distance(hero.pos, base) < 9500) {
		for (auto& mp: hero.threats) {
			if (isActiveTarget(mp.second) == false) {
				active_targets.insert(mp.second.id);
				return move(mp.second.pos) + " DEFENSE";
			}
		}
	}
	return move(hero.idle()) + " idle";
}

std::string Game::attackingStrat(const Entity& hero)
{
	return move(hero.idle()) + " idle";
}

std::string Game::generateAction(const Entity& hero)
{
	if (hero.is_attacker == true)
		return attackingStrat(hero);
	return defensiveStrat(hero);
}

//////////////
// ACTIONES //
//////////////

std::string	Game::move(const Position& pos) {
	return "MOVE " + std::to_string(pos.x) + " " + std::to_string(pos.y);
}

std::string	Game::windy_day(const Position& pos) {
	mana -= 10;
	return "SPELL WIND " + std::to_string(pos.x) + " " + std::to_string(pos.y);
}

std::string	Game::shield(const int id) {
	mana -= 10;
	return "SPELL SHIELD " + std::to_string(id);
}

std::string	Game::control(const int id, const Position& pos) {
	mana -= 10;
	int mod = rand() % 4500;
	mod *= (rand() % 2) ? 1 : -1;
	return "SPELL CONTROL " + std::to_string(id) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y + mod);
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
