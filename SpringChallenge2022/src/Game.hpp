
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

	/////////////
	// Helpers //
	/////////////
	public:

		bool	isClosestHero(const Entity& hero, const Position& pos);

		Entity	getClosestDangerousEntity(const Entity& hero);

	///////////
	// Logic //
	///////////
	public:

		std::string generateAction(const Entity& hero);

		std::string	defensiveStrat(const Entity& hero);

};
