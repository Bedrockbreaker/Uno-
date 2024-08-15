#include <fcntl.h>
#include <iostream>
#include <memory>

#include "Uno.h"

int main(int argc, char* argv[]) {
	// Enable UTF-8 output
	_setmode(_fileno(stdout), _O_U8TEXT);

	// Create the game in the heap
	const std::unique_ptr<Uno::Game> game = std::make_unique<Uno::Game>();
	
	game->printIntro();

	// Add players
	if (argc > 1) {
		// From command line
		for (int i = 1; i < argc; i++) {
			game->addPlayer(argv[i]);
		}
	} else {
		// Ask for each player
		int numPlayers = 0;
		
		std::wcout << "How many players?\n";
		
		do {
			std::cin >> numPlayers;
			std::cin.ignore(); // Ignore the newline character

			if (numPlayers <= 1) {
				std::wcout << "Please enter a number greater than 1\n";
			}
		} while (numPlayers <= 1);

		for (int i = 0; i < numPlayers; i++) {
			std::string name;
			std::wcout << "Enter player " << i + 1 << "'s name: ";
			getline(std::cin, name);
			game->addPlayer(name);
		}
	}

	// Start the game
	game->play();

	// Pause before exiting
	std::wcout << "Press enter to exit...\n";
	std::cin.get();
}