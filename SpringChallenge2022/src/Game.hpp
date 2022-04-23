
#pragma once

#include "Position.hpp"
#include "Entity.hpp"
#include <vector>

//////////
// Game //
//////////
class Game
{
	//////////////
	// Typedefs //
	//////////////
	public:

		typedef std::vector<Entity> vectity;

	//////////////////////
	// Member variables //
	//////////////////////
	public:

		Position	base;
		int			base_health;
		Position	enemy_base;
		int			enemy_health;
		int 		mana;
		int			enemy_mana;
		vectity		heroes;
		vectity		monsters;
		vectity		opponents;
		size_t		round_nb;

		static const Position default_hero_pos[];


	//////////////////
	// Construction //
	//////////////////
	private:

		Game(const Game& x);
		Game& operator = (const Game& x);

	public:

		Game();

	///////////////////
	// Funky Members //
	///////////////////
	private:

		void clearEntities();
		void setEntity(Entity& entity);

	public:

		void parseRound();

	///////////
	// Logic //
	///////////
	public:

		std::string generateAction(const Entity& hero);

};
