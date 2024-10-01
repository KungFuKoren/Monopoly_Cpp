//kkorenn1@gmail.com

#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include "Player.hpp"
#include "Board.hpp"

#include <SFML/Graphics.hpp>
#include <random>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>  
#include <array>

using namespace std;

class Gameplay {
private:
    vector<Player*> players;    // List of players in the game
    Board* board;               // Singleton Board instance
    int numPlayers;             // Number of players in the game
    int turn = 0;
    bool game = true;
    int cube1 = 0;
    int cube2 = 0;
    vector<Property*> properties;
    vector<Company*> companies;

public:
    

    Gameplay(int playerCount);    // Constructor to initialize game with playerCount players

    // Start the game, setup players, and initialize the board
    void startGame();

    // Destructor to clean up dynamically allocated memory
    ~Gameplay();

    // Display the current game state (players and board)
    void displayGameState() const;

    // Simulate a turn for a player
    void playTurn(Player* player);
    // Roll and move - tested
    void roll(); 
    void move(Player* player);
 
    // Building methods - tested
    bool canPurchase(Player* player); 
    void purchase(Player* player);  
    bool canBuildHouse(Player* player); 
    bool canBuildHouse(Property* prop, Player* player);
    bool canBuildHotel(Property* prop, Player* player);
    void buildHotel(Property* prop, Player* player);
    void buildHouse(Property* prop, Player* player);
    void buildHouse(Player* player); 
    bool ownsAllStreet(Player* player , vector<Property*>same_color); 
    bool canBuildHotel(Player* player); 
    void buildHotel(Player* player); 

    //Paying methods -tested
    void gottaPay(Player* player , Property* prop, Player* owner); 
    void gottaPay(Player* player , Company* comp, Player* owner); 
    void payToPlayer(Player* from , Player* to , int amount);
    void landedOnBlock(Player* player);

    //No money left removing player - tested 
    void declareBankruptcy(Player* player);

    //Simple methods to return needed stuff for the gameplay
    Player* getBlockPlayer(Block* block); 
    Block* getPlayerBlock(Player* player); 
    vector<Player*> getPlayers(); 
    Player* getNextPlayer();
    Board* getBoard();

    //Gets an input of y or n.
    bool getYesOrNoInput(); 

    //adds a card to player.
    bool getOutOfJailCard(Player* player);

    //Winning method - tested   
    bool CheckWin(); // checks if player is left alone or at 4000$
    void Turn(Player* player);
    void runGame(); // runs a while loop with checkwin at every round - calls turn .


    //Simple methods - 
    void goToJail(Player* player); // Moves played to jail and sets jail mode.
    void parking(Player* player);  //Gives turn to next player
    void setJail(Player* player);  // Adds the jail num of a player (number of turns hes in jail)
    void resetJail(Player* player); // sets jail num to 0
    // tries to leave jail with 3 doubles first , secondly asks if you want to use the get out of jail card ,
    // lastly if you wnat to spend 50%. when the 3rd turn comes and youre still in prison automaticly pays 50$.
    void tryToLeaveJail(Player* player); 
    //Gives out the options of what a player can do at the end of the turn.
    void whatCanIDo(Player* player);
    // Gives out chance card.
    void getChance(Player* player);
    //Method for me to use for tests.
    void setCubes(int x , int y);
    
};

#endif // GAMEPLAY_HPP
