
#include "Entity.hpp"
#include <iostream>

///////////////
// Operators //
///////////////

bool operator <  (const Entity& lhs, const Entity& rhs) {
	return (distance(lhs.pos, lhs.my_base) < distance(rhs.pos, rhs.my_base));
}

bool operator >  (const Entity& lhs, const Entity& rhs) {
	return (distance(lhs.pos, lhs.my_base) > distance(rhs.pos, rhs.my_base));
}

std::istream& operator >> (std::istream& is, Type& type) {
	int _type;
	is >> _type;
	type = static_cast<Type>(_type);
	return is;
}

std::istream& operator >> (std::istream& is, Target& target) {
	int _target;
	is >> _target;
	target = static_cast<Target>(_target);
	return is;
}

bool operator == (const Entity& lhs, const Entity& rhs)
{
	return (lhs.id == rhs.id);
}

/////////////
// Helpful //
/////////////

bool	Entity::isInRange4500(const Position& pos) const {
	return (distance(pos, this->pos) <= 4500);
}

bool	Entity::isInRange5000(const Position& pos) const {
	return (distance(pos, this->pos) <= 5000);
}

bool	Entity::isInRange6000(const Position& pos) const {
	return distance(pos, this->pos) <= 6000;
}

bool	Entity::isInRange7500(const Position& pos) const {
	return distance(pos, this->pos) <= 7500;
}

bool	Entity::isInSpellRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 2200);
}

bool	Entity::isInWindRange(const Position& pos) const {
	return (distance(pos, this->pos) <= 1280);
}

bool	Entity::movingToPos(const Position& pos) const {
	return distance(this->nextPos(), pos) <  distance(this->pos, pos);
}

Position	Entity::nextPos() const {
	Position ret;
	ret.x = this->pos.x + this->trajectory.x;
	ret.y = this->pos.y + this->trajectory.y;
	return ret;
}
