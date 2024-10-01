//kkorenn1@gmail.com
#ifndef BOARD_HPP
#define BOARD_HPP

#include "Player.hpp"
#include "Block.hpp"

#include <string>
#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <thread> 
//A singleton of a Board class ,
class Board {
private:

    static Board* instance;  
    std::vector<Player*> players;       
    std::vector<Block*> blocks;       // Array of Block pointers
    int size;                         // Size of the board (default is 40)
    vector<sf::Color> blockColors; // Store colors for each block added
    // Private constructor (singleton)
    Board(int boardSize = 40);
    sf::Color stringToColor(const std::string& colorStr) const;
    void fillBoard();
public:
    void removePlayer(const std::string& playerName);
    void setBlockOutlineColor(int index, sf::Color newColor);
    void addPlayer(Player* player);

    // Delete copy constructor and assignment operator to ensure singleton
    Board(const Board&) = delete;
    Board& operator=(const Board&) = delete;

    void createGameWindow(int width, int height);
    // Static method to get the instance of the board (default size is 40)
    static Board* getInstance(int boardSize = 40);

    // Destructor to clean up dynamically allocated blocks
    ~Board();

    // Method to display the board details
    void displayBoard() const;

    // Method to get a block at a specific position
    Block* getBlock(int index) const;

    // Method to set a block at a specific position
    void setBlock(int index, Block* block);

    // Method to get the size of the board
    int getSize() const;

    std::vector<Block*> getBlocks();
};

#endif // BOARD_HPP
