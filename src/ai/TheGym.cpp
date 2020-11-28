#include "../../include/ai/TheGym.h"
#include "../../include/ai/Dict.h"
#include "../../include/game/utility.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

//test comment

TheGym::TheGym() : 	
		bot1("bot1", 1),
		bot2("bot2", 2),
		g(){}

void TheGym::giveReward(){
	if(g.winner() == 1){
		bot1.feedReward(state, 1);
		bot2.feedReward(state, 0);
	}
	else if(g.winner() == 2){
		bot1.feedReward(state, 0);
		bot2.feedReward(state, 1);
	}
	else {
		bot1.feedReward(state, 0.1);
		bot2.feedReward(state, 0.5);
	}
}

void TheGym::reset(){
	bot1.reset();
	bot2.reset();
	g = Game(bot1, bot2);
}	

void print_states(Bot bot){
	vector<string> states;
	states = bot.getStates();

	for(int i = 0; i < states.size(); i++){
		cout << states[i] << endl;
	}
}

void TheGym::playRound(bool quiet){
	double winner;
	Dict<Move, string> options;
	Move next_move;

	while(!g.finished()) {
		if (quiet) std::cout << "bot 1's turn:\n";
		while(!g.finished() && g.whoseTurn() == 1){
			options = g.get_possibilities();
			if (quiet) std::cout << "\tchoosing move\n";
			next_move = bot1.chooseMove(state, options);
			if (quiet) std::cout << "\tmoving\n";
			g.makeMove(next_move.add_rem, next_move.x, next_move.y);
			bot1.add_state(serialize(g.flatten()));
		}
		if (quiet) std::cout << "bot 2's turn:\n";
		while(!g.finished() && g.whoseTurn() == 2){
			options = g.get_possibilities();
			if (quiet) std::cout << "\tchoosing move\n";
			next_move = bot2.chooseMove(state, options);
			if (quiet) std::cout << "\tmoving\n";
			g.makeMove(next_move.add_rem, next_move.x, next_move.y);
			bot2.add_state(serialize(g.flatten()));
		}
	}
	giveReward();

	//print_states(bot1);

	reset();
}

void TheGym::train(int rounds, bool quiet, bool start_fresh){
	std::cout << "Starting training...\n";
	if(!start_fresh){
		state.readPolicy();
	}
	for(int i = 0; i < rounds; i++){
		std::cout << "\nRound: " << i + 1 << endl;
		if(quiet){	
			beQuiet();
		}
		playRound(quiet);
	}
}

void TheGym::beQuiet(){
	bot1.quiet = true;
	bot2.quiet = true;
}

State TheGym::getState() const {
	return state;
}

void TheGym::savePolicy() {
	state.savePolicy();
}