#pragma once

#include <vector>

#include "Deck.h"
#include "Player.h"

namespace Uno {
	class Game {

	public:
		void addPlayer(const std::string& name);
		void advanceTurn();
		
		const Player& getCurrentPlayer() const;
		Player& getCurrentPlayer();
		const Player& getPreviousPlayer() const;
		Player& getPreviousPlayer();

		void play();
		void setup();

		void printIntro() const;
		
	private:
		Deck draw;
		Deck discard;
		std::vector<Player> players;
		int turn = 0;
		bool isClockwise = true;

		Card drawCard();
		std::vector<Card> drawCards(int amount);

		std::string getInput() const;

		bool handleDraw();
		bool handleRenege();
		bool handleChallenge();
		bool handleDiscard(const std::string& input);
		Player* checkForWinner();

		void clearScreen() const;
		void printCard(const Card& card) const;
		void printCards(std::vector<Card> cards) const;
		void printWaitScreen() const;
		void printCurrentPlayerScreen() const;
	};
}