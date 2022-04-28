
#pragma once

#include "Position.hpp"
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
