
#include "Action.hpp"

std::string	move(const Position& pos) {
	return "MOVE " + std::to_string(pos.x) + " " + std::to_string(pos.y);
}

std::string	windy_day(const Position& pos) {
	return "SPELL WIND " + std::to_string(pos.x) + " " + std::to_string(pos.y);
}

std::string	shield(const int id) {
	return "SPELL SHIELD " + std::to_string(id);
}

std::string	control(const int id, const Position& pos) {
	return "SPELL CONTROL " + std::to_string(id) + " " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " CONTROLAAAA";
}
