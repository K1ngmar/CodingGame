
#pragma once

#include "Position.hpp"
#include "Entity.hpp"
#include <vector>
#include <set>

//////////
// Game //
//////////
class Game
{
	//////////////
	// Typedefs //
	//////////////
	public:

		typedef std::vector<Entity>			vectity;
		typedef std::set<int>				target_map;
		typedef std::map<size_t, Entity>	dm;

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
		vectity		opponents;
		dm			monsters;
		size_t		round_nb;
		target_map	active_targets;
		target_map	controlled_enemies;
		

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

		void	clearEntities();
		void	setEntity(Entity& entity);

	public:

		void	createDistMap();
		void	parseRound();

	/////////////
	// Helpers //
	/////////////
	public:

		// bool isClosestTarget(const Entity& hero);
		bool isClosestHero(int id, Position& pos) const;
		bool annoyingEnemy(const Entity& target) const;
		Entity getAnnoyingEnemy(const Entity& target) const;

	///////////
	// Logic //
	///////////
	public:

		Entity		getBestDefendingTarget(const Entity& hero);
		Entity		getBestAttackingTarget(const Entity& hero);
		std::string generateAction(const Entity& hero);

	/////////////////
	/// Strategies //
	/////////////////
	public:

		std::string	defensiveStrat(const Entity& hero);
		std::string	attackingStrat(const Entity& hero);

};
