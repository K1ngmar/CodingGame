
#pragma once

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
