
#include "Position.hpp"

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
