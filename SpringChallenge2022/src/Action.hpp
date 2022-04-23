
#pragma once

#include "Position.hpp"
#include <string>

	std::string	move(const Position& pos);
	std::string	windy_day(const Position& pos);
	std::string	shield(const int id);
	std::string	control(const int id, const Position& pos);
