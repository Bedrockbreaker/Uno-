#include <algorithm>
#include <memory>

#include "Util.h"
#include "Player.h"

using namespace Uno;

// Standard constructor
Player::Player(std::string name) : name(name) {}

// getter name
const std::string& Player::getName() const {
	return name;
}

// getter score
int Player::getScore() const {
	return score;
}

// add score
void Player::addScore(int amount) {
	score += amount;
}

// getter hand (const)
const std::vector<Card>& Player::getHand() const {
	return hand;
}

// getter hand
std::vector<Card>& Player::getHand() {
	return hand;
}

// Try to get a card from a string input.
// Returns a pointer to the card if it exists, otherwise returns nullptr.
Card* Player::tryGetCard(const std::string& input) {
	for (auto& card : hand) {
		if (card.getDisplayColor() + card.getSymbol() == Util::toUpper_copy(input)) {
			return &card;
		}
	}

	return nullptr;
}

// Add a card to the player's hand
void Player::addCard(Card card) {
	hand.push_back(card);
}

// Add multiple cards to the player's hand
void Player::addCards(const std::vector<Card>& newCards) {
	hand.insert(hand.end(), newCards.begin(), newCards.end());
}

// getter renegeCard
const Card* Player::getRenegeCard() const {
	return renegeCard;
}

// clear renegeCard
void Player::clearRenegeCard() {
	renegeCard = nullptr;
}

// setter renegeCard
void Player::setRenegeCard(Card& card) {
	renegeCard = &card;
}

// Remove a card from the player's hand
void Player::removeCard(Card card) {
	for (auto it = hand.begin(); it != hand.end(); ++it) {
		if (*it == card) {
			hand.erase(it);
			break;
		}
	}
}

// Clear the player's hand
void Player::clearHand() {
	hand.clear();
}

// Check if the player just played a draw 4
bool Player::canChallenge() const {
	return playedDraw4;
}

// Set if the player just played a draw 4
void Player::setCanChallenge(bool canChallenge) {
	this->playedDraw4 = canChallenge;
}