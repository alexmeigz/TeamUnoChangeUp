#pragma once
#include "../game/Game.h"
#include <random>
#include <string>

#include <vector>
#include <utility>

class AIGame : public Game {
	public:
		AIGame(int playerId);
		bool makeMove(std::string move, int x, int y);
	private:
		void AIMakeMove();
		const int PLAYER_ID;
};
