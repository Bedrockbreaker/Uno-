#include <array>
#include <format>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Uno.h"
#include "Util.h"

using namespace Uno;

// Add a player to the game
void Game::addPlayer(const std::string& name) {
	players.push_back(Player(name));
}

// Advance to the next player
void Game::advanceTurn() {
	// Clear the previous player's canChallenge flag
	getPreviousPlayer().setCanChallenge(false);
	// Clear the current player's renege card
	getCurrentPlayer().clearRenegeCard();
	// Advance to the next player
	turn = (turn + (isClockwise ? 1 : -1) + players.size()) % players.size();
}

// Get the current player (const)
const Player& Game::getCurrentPlayer() const {
	return players[turn];
}

// Get the current player
Player& Game::getCurrentPlayer() {
	return players[turn];
}

// Get the previous player (const)
const Player& Game::getPreviousPlayer() const {
	return players[
		(turn + players.size() - (isClockwise ? 1 : -1)) % players.size()
	];
}

// Get the previous player
Player& Game::getPreviousPlayer() {
	return players[
		(turn + players.size() - (isClockwise ? 1 : -1)) % players.size()
	];
}

// Main gameplay loop
void Game::play() {

	// Setup the game

	setup();

	// Print the first wait screen and player screen

	printWaitScreen();
	printCurrentPlayerScreen();

	// Loop for an individual round
	while (true) {

		// Get input

		std::string input = getInput();

		// Handle input

		bool success;
		if (input == "draw") {
			success = handleDraw();
		} else if (input == "renege") {
			success = handleRenege();
		} else if (input == "challenge") {
			success = handleChallenge();
		} else {
			success = handleDiscard(input);
		}

		// If the game needs further input,
		// or the input was invalid, then continue

		if (!success) {
			continue;
		}

		// Check for a winner, break if so
		
		Player* winner = checkForWinner();
		if (winner != nullptr) {
			if (winner->getScore() >= 500) {
				break;
			} else {
				continue;
			}
		}

		// Advance the turn

		advanceTurn();
		printWaitScreen();
		printCurrentPlayerScreen();
	}
}

void Game::setup() {

	// Randomize player order

	Util::shuffle(players);

	draw.clear();
	discard.clear();

	// Create the deck

	for (int i = 0; i < (players.size() - 1) / 8 + 1; i++) {
		for (
			int colorInt = 0;
			colorInt < static_cast<int>(Color::COUNT) - 1;
			colorInt++
		) {
			Color color = static_cast<Color>(colorInt);

			draw.addCards({
				{ Color::WILD, Value::WILD },
				{ Color::WILD, Value::WILD_DRAW_FOUR },
				{ color, Value::ZERO },
				{ color, Value::DRAW_TWO }, { color, Value::DRAW_TWO },
				{ color, Value::SKIP }, { color, Value::SKIP },
				{ color, Value::REVERSE }, { color, Value::REVERSE },
			});

			for (
				int valueInt = 1;
				valueInt < static_cast<int>(Value::NINE) + 1;
				valueInt++
			) {
				Value value = static_cast<Value>(valueInt);

				draw.addCards({ { color, value }, { color, value } });
			}
		}
	}

	// Shuffle, ensuring no wild draw four is on top

	do {
		draw.shuffle();
	} while (draw.peak().getValue() == Value::WILD_DRAW_FOUR);
	
	// Draw and discard the first card

	discard.addCard(drawCard());

	// Reset game state

	isClockwise = true;
	turn = 0;

	// Deal 7 cards to each player and reset their state

	for (auto& player : players) {
		player.setCanChallenge(false);
		player.clearRenegeCard();
		player.getHand() = drawCards(7);
	}

	// If an action card was drawn, apply it

	switch (discard.peak().getValue()) {
		case Value::REVERSE:
			isClockwise = !isClockwise;
			advanceTurn();
			break;
		case Value::SKIP:
			advanceTurn();
			break;
		case Value::DRAW_TWO:
			getCurrentPlayer().addCards(drawCards(2));
			advanceTurn();
			break;
	}
}

// Draw a card, reshuffling if necessary
Card Game::drawCard() {
	if (draw.getCards().size() == 0) {
		std::vector<Card>& discardPile = discard.getCards();
		Card topDiscard = discard.drawCardUnchecked();
		draw.addCards(discardPile);
		draw.shuffle();
		discard.clear();
		discard.addCard(topDiscard);
	}

	return draw.drawCardUnchecked();
}

// Draw cards, reshuffling if necessary
std::vector<Card> Game::drawCards(int amount) {
	std::vector<Card> newCards;
	for (int i = 0; i < amount; i++) {
		newCards.push_back(drawCard());
	}
	return newCards;
}

// Get user input
std::string Game::getInput() const {
	std::string input;

	std::wcout << "\x1b[36m> \x1b[0m";
	std::getline(std::cin, input);

	return input;
}

// Handle user input to draw
bool Game::handleDraw() {
	Player& player = getCurrentPlayer();

	// Return if the player has already drawn a card

	if (player.getRenegeCard() != nullptr) {
		std::wcout << "You need to discard the last card you drew, or renege\n";
		return false;
	}

	// Draw 4 cards if a draw four is played against them

	if (getPreviousPlayer().canChallenge()) {
		player.addCards(drawCards(4));
		return true;
	}

	// Draw 1 card like normal

	player.addCard(drawCard());
	Card& card = player.getHand().back();
	player.setRenegeCard(card);

	// Return if the card doesn't match the discard pile

	if (!discard.cardMatches(card)) {
		return true;
	}

	// Ask the player if they want to discard the card

	std::wcout
		<< "You drew a "
		<< card.getDisplayColor() << Util::toWString(card.getSymbol())
		<< ", which matches the discard pile\n"
		<< "Discard the card, or enter \"renege\" to renege\n";

	return false;
}


// Handle user input to renege
bool Game::handleRenege() {

	// Return if the player hasn't drawn a card

	if (getCurrentPlayer().getRenegeCard() == nullptr) {
		std::wcout << "You can't renege right now\n";
		return false;
	}

	return true;
}

// Handle user input to challenge a draw 4
bool Game::handleChallenge() {

	// Return if the previous player can't be challenged

	if (!getPreviousPlayer().canChallenge()) {
		std::wcout << "You can't challenge right now\n";
		return false;
	}

	// Remove the draw 4 on the discard pile temporarily

	Card topCard = discard.getCards().back();
	discard.drawCardUnchecked();

	// Check if the prevous player could've played another card

	for (const auto& card : getPreviousPlayer().getHand()) {
		if (card.getValue() == Value::WILD_DRAW_FOUR) {
			continue;
		}

		// If they could, they draw 4, and the current player keeps their turn

		if (discard.cardMatches(card)) {
			discard.addCard(topCard);
			std::wcout
				<< "You won the challenge, forcing them to draw 4\n"
				<< "Their cards:\n";
			printCards(getPreviousPlayer().getHand());
			std::wcout << "Press enter to continue...\n";
			std::cin.get();
			getPreviousPlayer().addCards(drawCards(4));
			getPreviousPlayer().setCanChallenge(false);
			printCurrentPlayerScreen();
			return false;
		}
	}

	// If they couldn't, they draw 6, and the current player loses their turn

	discard.addCard(topCard);
	std::wcout
		<< "You lost the challenge and drew 6 cards\n"
		<< "Their cards:\n";
	printCards(getPreviousPlayer().getHand());
	std::wcout << "Press enter to continue...\n";
	std::cin.get();
	getCurrentPlayer().addCards(drawCards(6));
	getPreviousPlayer().setCanChallenge(false);
	return true;
}

// Handle user input to discard
bool Game::handleDiscard(const std::string& input) {

	// Return if a wild draw 4 has appeared

	if (getPreviousPlayer().canChallenge()) {
		std::wcout << "You can't play a card right now\n";
		return false;
	}

	Card* card = getCurrentPlayer().tryGetCard(input);

	// Return if the card doesn't exist

	if (card == nullptr) {
		std::wcout << "Can't find that card in your hand\n";
		return false;
	}

	// Return if the card doesn't match

	if (!discard.cardMatches(*card)) {
		std::wcout << "Card does not match\n";
		return false;
	}

	// Return if they attempt playing a different card than the one they just drew

	if (
		getCurrentPlayer().getRenegeCard() != nullptr
		&& *getCurrentPlayer().getRenegeCard() != *card
	) {
		std::wcout << "You need to discard the last card you drew, or renege\n";
		return false;
	}

	// Set the card's color if it's a wild card

	if (card->getColor() == Color::WILD) {
		std::string color;
		do {
			std::wcout
				<< "Choose color: (r)ed, (b)lue, (g)reen, (y)ellow, (c)ancel\n";
			color = getInput();

			if (
				color[0] != 'r'
				&& color[0] != 'b'
				&& color[0] != 'g'
				&& color[0] != 'y'
				&& color[0] != 'c'
			) {
				std::wcout << "Invalid color\n";
			}
		} while (
			color[0] != 'r'
			&& color[0] != 'b'
			&& color[0] != 'g'
			&& color[0] != 'y'
			&& color[0] != 'c'
		);

		switch (color[0]) {
			case 'r':
				card->setColor(Color::RED);
				break;
			case 'b':
				card->setColor(Color::BLUE);
				break;
			case 'g':
				card->setColor(Color::GREEN);
				break;
			case 'y':
				card->setColor(Color::YELLOW);
				break;
			case 'c':
				return false;
		}
	}

	// Copy the card, add it to the discard pile, and remove it from the player's hand

	Card cardCopy = *card;

	discard.addCard(*card);
	getCurrentPlayer().removeCard(*card);

	// Apply the effects of the card

	switch(cardCopy.getValue()) {
		case Value::DRAW_TWO:
			advanceTurn();
			getCurrentPlayer().addCards(drawCards(2));
			break;
		case Value::SKIP:
			advanceTurn();
			break;
		case Value::REVERSE:
			isClockwise = !isClockwise;
			if (players.size() == 2) {
				advanceTurn();
			}
			break;
		case Value::WILD_DRAW_FOUR:
			getCurrentPlayer().setCanChallenge(true);
			break;
	}

	return true;
}

// Check for a winner
Player* Game::checkForWinner() {

	// Return if the current and previous player's hand isn't empty

	if (
		getCurrentPlayer().getHand().size() != 0
		&& getPreviousPlayer().getHand().size() != 0
	) {
		return nullptr;
	}

	// Return if the current player can be challenged

	if (getCurrentPlayer().canChallenge()) {
		return nullptr;
	}

	// Get the winner

	Player* winner = &getCurrentPlayer();
	if (getCurrentPlayer().getHand().size() != 0) {
		winner = &getPreviousPlayer();
	}

	// Calculate the winner's score

	int prevScore = winner->getScore();

	for (const auto& player : players) {
		for (const auto& card : player.getHand()) {
			switch(card.getValue()) {
				case Value::ZERO:
				case Value::ONE:
				case Value::TWO:
				case Value::THREE:
				case Value::FOUR:
				case Value::FIVE:
				case Value::SIX:
				case Value::SEVEN:
				case Value::EIGHT:
				case Value::NINE:
					winner->addScore(
						static_cast<int>(card.getValue())
					);
					break;
				case Value::REVERSE:
				case Value::SKIP:
				case Value::DRAW_TWO:
					winner->addScore(20);
					break;
				case Value::WILD:
				case Value::WILD_DRAW_FOUR:
					winner->addScore(50);
					break;
				default:
					break;
			}
		}
	}

	// Print scores

	clearScreen();

	if (winner->getScore() >= 500) {
		printIntro();
		std::wcout
			<< Util::toWString(winner->getName())
			<< " wins the game with "
			<< winner->getScore()
			<< " points!\n";
		return winner;
	}

	std::wcout
		<< Util::toWString(winner->getName())
		<< " wins the round with "
		<< winner->getScore() - prevScore
		<< " points!\n";
	
	std::sort(
		players.begin(),
		players.end(),
		[](const Player& a, const Player& b) {
			return a.getScore() > b.getScore();
		}
	);

	for (const auto& player : players) {
		std::wcout
			<< Util::toWString(player.getName())
			<< " - "
			<< player.getScore()
			<< " points\n";
	}

	std::wcout << "Press enter to continue...\n";
	std::cin.get();

	// Start a new round if 500 points haven't been reached yet

	setup();
	printWaitScreen();
	printCurrentPlayerScreen();

	return winner;
}

// Clear the screen
void Game::clearScreen() const {
	for (int i = 0; i < 100; i++) {
		std::wcout << "\n";
	}
}

// Print a card
void Game::printCard(const Card& card) const {
	char color = card.getDisplayColor();
	std::string value = card.getDisplayValue();
	
	std::wstring cardText = std::format(
		L"╭─────╮\n│{0}    │\n│{1}   {2}│\n│{2}   {1}│\n│    {0}│\n╰─────╯",
		color,
		value[0],
		value[1]
	);

	std::wcout
		<< Util::toWString(Util::getColorCode(card.getColor()))
		<< cardText << L"\x1b[0m\n";
}

// Print cards
void Game::printCards(std::vector<Card> cards) const {
	std::array<std::wstring, 6> lines;

	// Sort by color, then value

	std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b) {
		return a.getColor() == b.getColor()
			? a.getValue() < b.getValue()
			: a.getColor() < b.getColor();
	});

	// Construct the lines for each card except the last

	for (int i = 0; i < cards.size() - 1; i++) {
		char color = cards[i].getDisplayColor();
		std::string value = cards[i].getDisplayValue();
		std::wstring colorCode = Util::toWString(
			Util::getColorCode(cards[i].getColor())
		);

		lines[0].append(colorCode + L"╭─");
		lines[1].append(colorCode + std::format(L"│{0}", color));
		lines[2].append(colorCode + std::format(L"│{0}", value[0]));
		lines[3].append(colorCode + std::format(L"│{0}", value[1]));
		lines[4].append(colorCode + L"│ ");
		lines[5].append(colorCode + L"╰─");
	}

	// Construct the lines for the last card

	char color = cards[cards.size() - 1].getDisplayColor();
	std::string value = cards[cards.size() - 1].getDisplayValue();
	std::wstring colorCode = Util::toWString(
		Util::getColorCode(cards[cards.size() - 1].getColor())
	);

	lines[0].append(colorCode + L"╭─────╮");
	lines[1].append(colorCode + std::format(L"│{0}    │", color));
	lines[2].append(colorCode + std::format(L"│{0}   {1}│", value[0], value[1]));
	lines[3].append(colorCode + std::format(L"│{0}   {1}│", value[1], value[0]));
	lines[4].append(colorCode + std::format(L"│    {0}│", color));
	lines[5].append(colorCode + L"╰─────╯");

	// Print the lines

	for (int i = 0; i < 6; i++) {
		std::wcout << lines[i] << "\n";
	}

	std::wcout << "\x1b[0m";
}

// Print wait screen
void Game::printWaitScreen() const {
	clearScreen();

	// Print the players in turn order and their hand sizes

	for (int i = 0; i < players.size(); i++) {
		const Player& player = players[i];

		std::wstring text = Util::toWString(player.getName())
			+ L" (" + std::to_wstring(player.getHand().size()) + L")";

		if (i == turn) {
			text = L"\x1b[7m" + text + L"\x1b[0m";
		}

		if (i != players.size() - 1) {
			text = text + (isClockwise ? L" -> " : L" <- ");
		}

		std::wcout << text;
	}

	// Print the current player's turn

	std::wcout
		<< "\nIt is now "
		<< Util::toWString(getCurrentPlayer().getName())
		<< "'s turn.\n"
		<< "Press enter to continue...\n";
	std::cin.get();
}

// Print logo
void Game::printIntro() const {
	std::wcout
		<< "\n"
		<< ":::    ::: ::::    :::  ::::::::                              \n"
		<< ":+:    :+: :+:+:   :+: :+:    :+:      :+:           :+:      \n"
		<< "+:+    +:+ :+:+:+  +:+ +:+    +:+      +:+           +:+      \n"
		<< "+#+    +:+ +#+ +:+ +#+ +#+    +:+ +#++:++#++:++ +#++:++#++:++ \n"
		<< "+#+    +#+ +#+  +#+#+# +#+    +#+      +#+           +#+      \n"
		<< "#+#    #+# #+#   #+#+# #+#    #+#      #+#           #+#      \n"
		<< " ########  ###    ####  ########                              \n"
		<< "\n";
}

// Print the current player's screen (hand and discard pile)
void Game::printCurrentPlayerScreen() const {
	const std::vector<Card>& cards = getCurrentPlayer().getHand();

	// Print their hand and the discard pile

	printCard(discard.peak());
	printCards(cards);

	// Get a random card

	const Card& randomCard = cards[Util::randInt(0, cards.size() - 1)];

	// Print the prompt, depending on if they can challenge

	if (getPreviousPlayer().canChallenge()) {
		const std::vector<Card>& discardPile = discard.getCards();
		const Card& lastCard = discardPile[discardPile.size() - 2];

		std::wcout
			<< "A Wild Draw 4 has appeared!\n"
			<< "The previous card was a "
			<< lastCard.getDisplayColor()
			<< Util::toWString(lastCard.getSymbol())
			<< "\nEnter \"challenge\" to challenge it\n";
	} else {
		std::wcout
			<< "Enter a card to play (e.g. \""
			<< randomCard.getDisplayColor()
			<< Util::toWString(randomCard.getSymbol())
			<< "\")\n";
	}
	std::wcout << "Or enter \"draw\" to draw\n";
}