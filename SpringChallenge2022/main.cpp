#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


/*
	TYPES
*/

struct Position {
	int x;
	int y;
};

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
	bool		is_targeting;
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

/*
	ACTUAL CODE
*/

static void init()
{
	std::cin >> base.x >> base.y; std::cin.ignore();
}

static void parseRound()
{
	int		entitiy_count;
	Entity	entity;
	int		type;
	int		target;

	monsters.clear();
	heroes.clear();
	opponents.clear();
	std::cin >> entitiy_count; std::cin.ignore();
	std::cerr << "entity count: " << entitiy_count << std::endl;
	for (int i = 0; i < entitiy_count; ++i)
	{
		std::cin >> entity.id;
		std::cin >> type;
		entity.type = static_cast<Entity::Type>(type);
		std::cin >> entity.pos.x >> entity.pos.y;
		std::cin >> entity.shield_life;
		std::cin >> entity.is_controlled;
		std::cin >> entity.health;
		std::cin >> entity.trajectory.x >> entity.trajectory.y;
		std::cin >> entity.is_targeting;
		std::cin >> target; std::cin.ignore();
		entity.target = static_cast<Entity::Target>(target);

		std::cout << "entity type: " << entity.type << std::endl;
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
}

int main()
{
	Entity hero;
	init();

	// game loop
	while (1)
	{
		parseRound();
		for (size_t i = 0; i < heroes.size(); ++i) {
			hero = heroes[i];
			// Write an action using cout. DON'T FORGET THE "<< endl"
			// To debug: cerr << "Debug messages..." << endl;


			// In the first league: MOVE <x> <y> | WAIT; In later leagues: | SPELL <spellParams>;
			std::cout << "WAIT" << std::endl;
		}
	}
}