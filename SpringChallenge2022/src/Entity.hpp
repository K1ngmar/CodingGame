
#pragma once

#include "Position.hpp"
#include <istream>

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
	Position	default_pos; // only set for heroes
	Type		type;
	Position	pos;
	int 		shield_life;
	int 		is_controlled;
	int			health;
	Position	trajectory;
	int			is_targeting;
	Target		target;

//////////////////
// Construction //
//////////////////

	Entity();

/////////////
// Helpful //
/////////////
public:

	bool	isCloseToPos(const Position& pos) const;
	bool	isInSpellRange(const Position& pos) const;
	bool	isInWindRange(const Position& pos) const;
	
	Position	nextPos() const;

}; /* end of Entity class */

///////////////
// Operators //
///////////////

	std::istream& operator >> (std::istream& is, const Entity::Type& type);
	std::istream& operator >> (std::istream& is, const Entity::Target& target);
