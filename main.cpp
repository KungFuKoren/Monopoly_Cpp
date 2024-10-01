//kkorenn1@gmail.com

#include <iostream>
#include "Gameplay.hpp"


using namespace std;

int main() {
    int numPlayers;

    cout << "Welcome to Monopoly!" << endl;
    cout << "Enter the number of players (2-8): ";
    cin >> numPlayers;

    // Validate player count
    if (numPlayers < 2 || numPlayers > 8) {
        cout << "Invalid number of players. Please enter a number between 2 and 8." << endl;
        return 1;
    }

    // Create the game
    Gameplay game(numPlayers);

    // Start the game
    game.startGame();
    
    game.runGame();
    



    return 0;
}
