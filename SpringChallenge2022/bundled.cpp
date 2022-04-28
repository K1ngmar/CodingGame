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

#define DEBUG 1

#if DEBUG == 1
    #define TRACE() dprintf(2, "%s:%d\n", __FUNCTION__,  __LINE__)
#else
    #define TRACE()
#endif

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

struct Entity
{

///////////////
// Variables //
///////////////

	int			id;
	Position	default_pos;
	Type		type;
	Position	pos;
	int 		shield_life;
	int 		is_controlled;
	int			health;
	Position	trajectory;
	int			is_targeting;
	Target		target;
	Position	my_base;
	Entity*		current_target;

/////////////
// Helpful //
/////////////
public:

	bool	isInRange4500(const Position& pos) const;
	bool	isInRange5000(const Position& pos) const;
	bool	isInRange6000(const Position& pos) const;
	bool	isInRange7500(const Position& pos) const;

	bool	isInSpellRange(const Position& pos) const;
	bool	isInWindRange(const Position& pos) const;

	bool	movingToPos(const Position& pos) const;

	Position	nextPos() const;

}; /* end of Entity class */

///////////////
// Operators //
///////////////

	bool operator == (const Entity& lhs, const Entity& rhs);

	bool operator <  (const Entity& lhs, const Entity& rhs);
	bool operator >  (const Entity& lhs, const Entity& rhs);

	std::istream& operator >> (std::istream& is, const Type& type);
	std::istream& operator >> (std::istream& is, const Target& target);

/*-- #include "src/Entity.hpp" end --*/
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

/*-- #include "src/Game.hpp" end --*/
#include <iostream>

int main()
{
	Game		game;

	while(true)
	{
		game.parseRound();
		for (Entity& hero: game.heroes) {
			game.performAction(hero);
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

bool operator <  (const Entity& lhs, const Entity& rhs) {
	return (distance(lhs.pos, lhs.my_base) < distance(rhs.pos, rhs.my_base));
}

bool operator >  (const Entity& lhs, const Entity& rhs) {
	return (distance(lhs.pos, lhs.my_base) > distance(rhs.pos, rhs.my_base));
}

std::istream& operator >> (std::istream& is, Type& type) {
	int _type;
	is >> _type;
	type = static_cast<Type>(_type);
	return is;
}

std::istream& operator >> (std::istream& is, Target& target) {
	int _target;
	is >> _target;
	target = static_cast<Target>(_target);
	return is;
}

bool operator == (const Entity& lhs, const Entity& rhs)
{
	return (lhs.id == rhs.id);
}

/////////////
// Helpful //
/////////////

bool	Entity::isInRange4500(const Position& pos) const {
	return (distance(pos, this->pos) <= 4500);
}

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

/*-- File: src/Entity.cpp end --*/
/*-- File: src/Game.cpp start --*/

/*-- #include "src/Game.hpp" start --*/
/*-- #include "src/Game.hpp" end --*/
#include <iostream>
#include <algorithm>
/*-- #include "src/Entity.hpp" start --*/
/*-- #include "src/Entity.hpp" end --*/

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

Game::Game() : round(0)
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
		// if (round == 0)
			setEntity(entity);
		// else
		// 	update(entity);
	}
	std::sort(monsters.begin(), monsters.end());
	++round;
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
	permutePair tmp[3];
	permutePair	best[3];
	Entity*		monster;

	for (size_t offset = 0; offset < HERO_AMT; offset++) {
		for (size_t j = 1; j < HERO_AMT; j++) {
			rating = 0;
			for (size_t i = 0; i < HERO_AMT; i++) {
				if ((i * j) % HERO_AMT > monsters.size())
					monster = NULL;
				else
					monster = &(monsters[(i * j) % HERO_AMT]);
				tmp[i] = {
						&(heroes[(i + offset) % HERO_AMT]),
						monster
					};
				if (monster)
					rating += distance(tmp[i].hero->pos, monster->pos);
			}
			if (rating < best_rating) {
				for (size_t i = 0; i < HERO_AMT; i++)
					best[i] = {tmp[i].hero, tmp[i].target};
			}
		}
	}
	for (size_t i = 0; i < HERO_AMT; i++) {
		best[i].hero->current_target = best[i].target;
	}
}

// bool	Game::stoopidAttack(Entity& hero)
// {
// 	if (monsters.size() > 0) {
// 		for (const Entity& monster: monsters) {
// 			if (std::find(active_targets.begin(), active_targets.end(), monster) == active_targets.end()) {
// 				move(monster.pos, " DEFENDING");
// 				active_targets.push_back(monster);
// 				return true;
// 			}
// 		}
// 	}
// 	return false;
// }

bool	Game::attackTarget(const Entity& hero)
{
	// if (hero.current_target != NULL) {
	// 	if (mana >= 10 && hero.current_target->shield_life == 0) {
	// 		if (hero.current_target->isInRange4500(base) && hero.current_target->isInWindRange(hero.pos))
	// 			return windy_day(center, " WOOSH");
	// 		else if (hero.current_target->isInRange5000(base) && hero.current_target->isInSpellRange(hero.pos))
	// 			return control(hero.current_target->id, enemy_base);
	// 	}
	if (hero.current_target)
		return move(hero.current_target->pos, " PERM");
	// }
	// return false;
}

bool	Game::defend(Entity& hero)
{
	bool attacked = false;

	attacked = attacked || attackTarget(hero);
	return attacked;
}

bool	Game::moveDefaultPos(Entity& hero)
{
	int				radius = 1600;
	Position		middle = hero.default_pos;
	Position		goal;
	static float tatta = 0;
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
	std::cout << "SPELL CONTROL " << id << " " << pos.x << " " << pos.y << addon << std::endl;
	return true;
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
