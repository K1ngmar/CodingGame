#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

/*
	typedefs
*/

	struct Entity;

	typedef std::vector<Entity> vectity;

/*
	TYPES
*/

struct Position {
	int x;
	int y;
};

bool operator == (const Position& lhs, const Position& rhs) {
	return (lhs.x == rhs.x && lhs.y == rhs.y);
}

struct Entity {

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
	Type		type;
	Position	pos;
	int 		shield_life;
	int 		is_controlled;
	int			health;
	Position	trajectory;
	int			is_targeting;
	Target		target;

}; /* end of Entity class */

/*
	CLASSES
*/



/*
	Global Variables
*/

Position		base;
int				base_health;
int 			mana;
vectity			heroes;
vectity			monsters;
vectity			opponents;
std::set<int>	active_targets;
static const Position default_hero_pos[] = {
		{5000, 800},
		{3500, 3500},
		{1500, 5000},
	};

/*
	ACTUAL CODE
*/

static void init()
{
	int dont_need_this_but_has_to_be_read_otherwise_things_will_go_wrong;
	std::cin >> base.x >> base.y; std::cin.ignore();
	std::cin >> dont_need_this_but_has_to_be_read_otherwise_things_will_go_wrong; std::cin.ignore();
}

static void clear_entities()
{
	monsters.clear();
	heroes.clear();
	opponents.clear();
	active_targets.clear();
}

static void set_entity(Entity& entity)
{
	switch(entity.type)
	{
		case Entity::MONSTER:
			monsters.push_back(entity); break;
		case Entity::HERO:
			heroes.push_back(entity); break;
		case Entity::OPPONENT:
			opponents.push_back(entity); break;
	}
}

static void parse_round()
{
	int		entity_count;
	Entity	entity;
	int		type;
	int		target;
	int 	bh, mn;
	
	std::cin >> base_health >> mana; std::cin.ignore();
	std::cin >> bh >> mn; std::cin.ignore();
		
	std::cin >> entity_count; std::cin.ignore();

	clear_entities();
	for (int i = 0; i < entity_count; ++i)
	{
		std::cin >> entity.id \
		>> type \
		>> entity.pos.x \
		>> entity.pos.y \
		>> entity.shield_life \
		>> entity.is_controlled \
		>> entity.health \
		>> entity.trajectory.x \
		>> entity.trajectory.y \
		>> entity.is_targeting \
		>> target;
		std::cin.ignore();
		entity.type = static_cast<Entity::Type>(type);
		entity.target = static_cast<Entity::Target>(target);

		set_entity(entity);
	}
}

size_t		distance(const Position& h, const Position& e)
{
	return (abs(h.x - e.x) + abs(h.y - e.y));
}

/*
	LOGIC
*/

bool		is_closest_hero(const Entity& hero, const Position& pos)
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

Entity		close_to_dangerous_entity(const Entity& cur)
{
	Entity closest;
	size_t closest_dist = 420691337;

	for (vectity::iterator i = monsters.begin(); i != monsters.end(); ++i) {
		if (is_closest_hero(cur, i->pos)) {
			if (distance(base, i->pos) < closest_dist) {
				closest_dist = distance(base, i->pos);
				closest = *i;
			}
		}
	}
	return closest;
}

bool		too_far_from_base(const Entity& hero)
{
	size_t dist = distance(hero.pos, base);

	return (dist > 8500);
}

Position	next_pos(const Entity& entity)
{
	Position next_pos;
	next_pos.x = entity.pos.x + entity.trajectory.x;
	next_pos.y = entity.pos.y + entity.trajectory.y;
	return (next_pos);
}

std::string	move_pos(const Position& pos)
{
	return "MOVE " + std::to_string(pos.x) + " " + std::to_string(pos.y);
}

bool		is_moving_to_base(const Entity& entity)
{
	return distance(next_pos(entity), base) <  distance(entity.pos, base);
}

bool		heroes_spread(const Entity& hero)
{
	size_t dist;
	for (vectity::iterator i = heroes.begin(); i != heroes.end(); ++i) {
		dist = distance(hero.pos, i->pos);
		if (dist < 1500 && dist > 0)
			return false;
	}
	return true;
}

Entity		get_closest_hero(const Position& pos)
{
	Entity	hero;
	size_t	dist = 420691337;
	for (vectity::iterator i = heroes.begin(); i != heroes.end(); ++i) {
		if (distance(hero.pos, i->pos) < dist && distance(hero.pos, i->pos) > 0) {
			dist = distance(hero.pos, i->pos);
			hero = *i;
		}
	}
	return hero;
}

std::string	spread_heroes(const Entity& hero)
{
	Entity closest = get_closest_hero(hero.pos);
	int idx = 0;
	while (heroes[idx].id != hero.id)
		++idx;
	return move_pos(default_hero_pos[idx]);
}

std::string	windy_day()
{
	return "SPELL WIND 8900 4500";
}

std::string	generate_move(const Entity& hero)
{
	if (too_far_from_base(hero) == true) {
		Entity closest = close_to_dangerous_entity(hero);
		// attack if entity is moving towards the base
		if (is_moving_to_base(closest) == true) {
			return move_pos(closest.pos);
			active_targets.insert(closest.id);
		}
		// return to base
		else {
			return move_pos(base);
		}
	}
	// hero is not too far from base
	else {
		if (monsters.size() > 0) {
			Entity closest = close_to_dangerous_entity(hero);
			if (distance(closest.pos, hero.pos) <= 1280 && mana > 10) {
				return windy_day();
			}
			if (is_moving_to_base(closest) == true) {
				if (distance(closest.pos, base) < 2500 || active_targets.find(closest.id) == active_targets.end()) {
					active_targets.insert(closest.id);
					return move_pos(closest.pos); // maybe next pos?
				}
				// find best target
				else {
					return move_pos(closest.pos); // make new function
				}
			}
			else {
				return spread_heroes(hero); // spread heroes
			}
		}
		// spread heroes
		else {
			return spread_heroes(hero);
		}
	}
}

int main()
{
	init();
	// game loop
	while (1)
	{
		parse_round();
		for (size_t i = 0; i < heroes.size(); ++i)
		{
			std::cout << generate_move(heroes[i]) << std::endl;
		}
	}
}
