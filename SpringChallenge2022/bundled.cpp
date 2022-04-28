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
	bool		is_attacker;

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
#include <set>

# define HERO_AMT 3

struct Game
{
// statitcs

	static const Position base_positions[];
	static const Position center;

// typedefs

	typedef std::vector<Entity> vectity;
	typedef std::set<size_t>	sset;

// variables

	Position	base;
	Position	enemy_base;
	int			defender_amt;
	int			health;
	int			enemy_health;
	int			mana;
	int			enemy_mana;
	int			round;

	vectity		heroes;
	vectity		monsters;
	vectity		opponents;
	vectity		active_targets;
	sset		controlled;

// construct

	Game();

	Entity	readEntity();
	void	setEntity(Entity& entity);
	void	update(Entity& entity);
	void	parseRound();
	void	clearEntities();

// logic

	void	permutationNation();

	Entity*	findBestAttackEntity(const Entity& hero);
	bool	attack(const Entity& hero);

	bool	useSpell(const Entity& hero);
	bool	annoyingEnemy(const Entity& hero);
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
				if ((i * j) % defender_amt >= monsters.size())
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
				best_rating = rating;
				for (size_t i = 0; i < defender_amt; i++)
					best[i] = {tmp[i].hero, tmp[i].target};
			}


			std::cerr << "---- " << rating << " ----" << std::endl;
			std::cerr << tmp[0].hero->id << ", " << tmp[0].target->id << ", pos: " << tmp[0].target->pos.x << ", " << tmp[0].target->pos.y << std::endl;
			if (tmp[1].hero && tmp[1].target)
				std::cerr << tmp[1].hero->id << ", " << tmp[1].target->id << ", pos: " << tmp[1].target->pos.x << ", " << tmp[1].target->pos.y << std::endl;
			if (tmp[2].hero && tmp[2].target)
				std::cerr << tmp[2].hero->id << ", " << tmp[2].target->id << ", pos: " << tmp[2].target->pos.x << ", " << tmp[2].target->pos.y << std::endl;
			std::cerr << std::endl;

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

Entity*	Game::findBestAttackEntity(const Entity& hero)
{
	Entity* best = NULL;

	for (Entity& monster: monsters) {
		if (monster.isInSpellRange(hero.pos) && monster.shield_life == 0) {
			if (best == NULL)
				best = &monster;
			else if (controlled.find(monster.id) == controlled.end())
				best = &monster; // can now shield
			else if (monster.health > best->health)
				best = &monster;
		}
	}
	return best;
}

bool	Game::attack(const Entity& hero)
{
	Entity* monster;

	if (defender_amt < 3 && hero.is_attacker) {
		if (mana > 50) {
			monster = findBestAttackEntity(hero);
			if (monster) {
				if (controlled.find(monster->id) == controlled.end())
					return (control(monster->id, enemy_base));
				else
					return shield(monster->id);
			}			
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
	int y_offset = (rand() % 5000) * ((rand() % 2) ? 1 : -1);
	std::cout << "SPELL CONTROL " << id << " " << pos.x << " " << pos.y + y_offset << addon << std::endl;
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
