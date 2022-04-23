
#include "Entity.hpp"
#include "Action.hpp"
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
	Position ret;
	std::cerr << "x: " << this->trajectory.x << ", y: " << this->trajectory.y << std::endl;
	ret.x = this->pos.x + this->trajectory.x;
	ret.y = this->pos.y + this->trajectory.y;
	return ret;
}

std::string	Entity::moveDefaultPos() const
{
	if (this->is_attacker == true)
		return move(center);
	return move(this->default_pos);
}
