#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

/*
	typedefs
*/

	struct Entity;

	typedef std::map<size_t, Entity> map_rating;

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

Position			base;
int					base_health;
int 				mana;
std::vector<Entity>	heroes;
std::vector<Entity>	monsters;
std::vector<Entity>	opponents;
std::vector<Position> active_targets;

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

	for (int i = 0; i < 2; ++i) {
		std::cin >> base_health >> mana; std::cin.ignore();
	}
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

size_t		calc_distance(Position& h, Position& e)
{
	return (abs(h.x - e.x) + abs(h.y - e.y));
}

map_rating	create_rating()
{
	map_rating	rating;
	size_t		dist;

	for (size_t i = 0; i < monsters.size(); ++i) {
		dist = calc_distance(base, monsters[i].pos);
		rating[dist] = monsters[i];
	}
	return rating;
}

static void	walk_pos(Position& target, bool& moved)
{
	moved = true;
	active_targets.push_back(target);
	std::cout << "MOVE " << target.x << " " << target.y << std::endl;
}

int main()
{
	Entity hero;
	map_rating mappert;
	map_rating::iterator moves;
	bool moved;
	int dist;

	init();
	// game loop
	while (1)
	{
		parse_round();
		mappert = create_rating();
		for (size_t i = 0; i < heroes.size(); ++i)
		{
			moves = mappert.begin();
			moved = false;
			for (size_t j = 0; j < 3; j++)
			{
				if (moves != mappert.end())
				{
					dist = calc_distance(base, moves->second.pos);
					if (dist < 3000) {
						walk_pos(moves->second.pos, moved);
						break ;
					}
					else if (dist < 7000 && std::find(active_targets.begin(), active_targets.end(), moves->second.pos) == active_targets.end()) {
						walk_pos(moves->second.pos, moved);
						break;
					}
					else {
						++moves;
						continue ;
					}
					break ;
				}
			}
			if (moved == false)
				std::cout << "WAIT" << std::endl;

			// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
		}
	}
}
