#include "Card.h"
#include "Deck.h"
#include "Util.h"

using namespace Uno;

// getter cards (const)
const std::vector<Card>& Deck::getCards() const {
	return cards;
}

// getter cards
std::vector<Card>& Deck::getCards() {
	return cards;
}

// shuffle the deck
void Deck::shuffle() {
	Util::shuffle(cards);
}

// Check if a card matches the top card
bool Deck::cardMatches(const Card& card) const {
	Card topCard = peak();

	return topCard.getColor() == card.getColor()
		|| topCard.getValue() == card.getValue()
		|| card.getColor() == Color::WILD
		|| topCard.getColor() == Color::WILD;
}

// Draw a card. Throws an error if the deck is empty
Card Deck::drawCardUnchecked() {
	Card card = cards.back();
	cards.pop_back();

	if (
		card.getValue() == Value::WILD
		|| card.getValue() == Value::WILD_DRAW_FOUR
	) {
		card.setColor(Color::WILD);
	}

	return card;
}

// Draw cards. Throws an error if the deck is empty
std::vector<Card> Deck::drawCardsUnchecked(int amount) {
	std::vector<Card> newCards;
	for (int i = 0; i < amount; i++) {
		newCards.push_back(drawCardUnchecked());
	}
	return newCards;
}

// Add a card
void Deck::addCard(Card card) {
	cards.push_back(card);
}

// Add cards
void Deck::addCards(std::vector<Card> newCards) {
	cards.insert(cards.end(), newCards.begin(), newCards.end());
}

// Clear the deck
void Deck::clear() {
	cards.clear();
}

// Get the top card (const)
const Card& Deck::peak() const {
	return cards.back();
}

// Get the top card
Card& Deck::peak() {
	return cards.back();
}