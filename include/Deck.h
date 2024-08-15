#pragma once

#include <vector>

#include "Card.h"

namespace Uno {
	class Deck {

	public:
		const std::vector<Card>& getCards() const;
		std::vector<Card>& getCards();
		void shuffle();
		bool cardMatches(const Card& card) const;

		Card drawCardUnchecked();
		std::vector<Card> drawCardsUnchecked(int amount);

		void addCard(Card card);
		void addCards(std::vector<Card> newCards);

		void clear();
		
		const Card& peak() const;
		Card& peak();

	private:
		std::vector<Card> cards;
	};
}