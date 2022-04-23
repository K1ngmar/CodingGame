
#pragma once

#include "Position.hpp"
#include <istream>

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

	bool	movingToPos(const Position& pos) const;

	Position	nextPos() const;

	std::string	moveDefaultPos() const;

}; /* end of Entity class */

///////////////
// Operators //
///////////////

	std::istream& operator >> (std::istream& is, const Entity::Type& type);
	std::istream& operator >> (std::istream& is, const Entity::Target& target);
