#pragma once

#include <memory>
#include <string>
#include <optional>
#include <vector>

#include "Card.h"

namespace Uno {
	class Player {

	public:
		Player(std::string name);

		const std::string& getName() const;

		int getScore() const;
		void addScore(int amount);

		const std::vector<Card>& getHand() const;
		std::vector<Card>& getHand();

		Card* tryGetCard(const std::string& input);
		
		void addCard(Card card);
		void addCards(const std::vector<Card>& newCards);
		void removeCard(Card card);
		void clearHand();

		const Card* getRenegeCard() const;
		void clearRenegeCard();
		void setRenegeCard(Card& card);

		bool canChallenge() const;
		void setCanChallenge(bool canChallenge);

	private:
		std::string name;
		int score = 0;
		std::vector<Card> hand;
		Card* renegeCard = nullptr;
		bool playedDraw4 = false;
	};
}