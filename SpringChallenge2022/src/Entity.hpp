
#pragma once

#include "Position.hpp"
#include <istream>
#include <map>

static const Position center = {8900, 4500};

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
	bool		is_updated;

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

}; /* end of Entity class */

///////////////
// Operators //
///////////////

	bool operator == (const Entity& lhs, const Entity& rhs);

	std::istream& operator >> (std::istream& is, const Type& type);
	std::istream& operator >> (std::istream& is, const Target& target);
