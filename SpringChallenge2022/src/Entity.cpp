
#include "Entity.hpp"
#include <iostream>

///////////////
// Operators //
///////////////

std::istream& operator >> (std::istream& is, Entity::Type& type) {
	int _type;
	is >> _type;
	type = static_cast<Entity::Type>(_type);
	return is;
}

std::istream& operator >> (std::istream& is, Entity::Target& target) {
	int _target;
	is >> _target;
	target = static_cast<Entity::Target>(_target);
	return is;
}

//////////////////
// Construction //
//////////////////

Entity::Entity()
{
	std::cin >> this->id \
	>> this->type \
	>> this->pos.x \
	>> this->pos.y \
	>> this->shield_life \
	>> this->is_controlled \
	>> this->health \
	>> this->trajectory.x \
	>> this->trajectory.y \
	>> this->is_targeting \
	>> this->target;
	std::cin.ignore();
}

/////////////
// Helpful //
/////////////

bool	Entity::isCloseToPos(const Position& pos) const {
	return (distance(pos, this->pos) <= 6000);
}

bool	Entity::isInSpellRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 2200);
}

bool	Entity::isInWindRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 1280);
}

Position	Entity::nextPos() const {
	return {this->pos.x + this->trajectory.x,
			this->pos.y + this->trajectory.y};
}
