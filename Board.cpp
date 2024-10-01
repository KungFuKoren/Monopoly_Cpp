//kkorenn1@gmail.com
#include "Board.hpp"
#include <iostream>

// Initialize the static instance pointer to nullptr
Board* Board::instance = nullptr;

// Private constructor to initialize an empty board with nullptr blocks
Board::Board(int boardSize) : size(boardSize) {
    blocks.resize(size, nullptr);  // Initialize the board with nullptr blocks
    blockColors.resize(size, sf::Color::Transparent);
    fillBoard();
} 


// Singleton getInstance method
Board* Board::getInstance(int boardSize) {
    if (instance == nullptr) {
        instance = new Board(boardSize);  // Create a new Board with default or custom size
    }
    return instance;
}

// Destructor to clean up dynamically allocated blocks
Board::~Board() {
    for (auto block : blocks) {
        delete block;  // Free memory for each block
    }
    blocks.clear();
    for (auto player : players) {
        delete player;  // Free memory for each player
    }
    players.clear();
}

void Board::removePlayer(const std::string& playerName) {
    auto it = std::remove_if(players.begin(), players.end(), [&playerName](Player* player) {
        return player->getName() == playerName;
    });

    // Erase the removed player from the vector
    if (it != players.end()) {
        players.erase(it, players.end());
    }
}


void Board::setBlockOutlineColor(int index, sf::Color newColor) {
    if (index >= 0 && index < size) {
        blockColors[index] = newColor; // Update the color for the block
    }
}
void Board::addPlayer(Player* player){
    players.push_back(player);
}

//fills the board according to the picture.
void Board::fillBoard(){
    blocks[0] = new GeneralBlock("Start", 695, 695,799,799,0);
    blocks[1] = new Property("Mediterranean avenue" , "Brown" ,60 , 20 ,630 ,695 ,695,799,1 );
    blocks[2] = new GeneralBlock("Treasure",565,695,630,799,2);
    blocks[3] = new Property("Baltic avenue" , "Brown" , 60 , 20,500,695,565,799,3);
    blocks[4] = new GeneralBlock("Tax",435,695,500,799,4); 
    blocks[5] = new Company("Rail Reading" , 200 , 0,370,695,435,799,5);
    blocks[6] = new Property("Oriental avenue" , "Teal",100 , 30, 305,695,370,799,6);
    blocks[7] = new Chance("Chance", 240,695,305,799,7);
    blocks[8] = new Property("Vermont avenue" , "Teal",100 , 30,175,695,240,799,8);
    blocks[9] = new Property("Connecticut avenue" , "Teal",120 , 40,110,695,175,799,9);
    blocks[10] = new GeneralBlock("Jail",0,695,105,799,10);
    blocks[11] = new Property("St. Charles Place" , "Pink",140,45, 0,630,105,695,11);
    blocks[12] = new Company("Electric Company" , 150 ,0,0,565,105,630,12);
    blocks[13] = new Property("States avenue" , "Pink",140,45, 0,500,105,565,13);
    blocks[14] = new Property("Virginia avenue" , "Pink",160,50 ,0,435,105,500,14);
    blocks[15] = new Company("Rail Pennsylvania" , 200 , 0, 0,370,105,435,15);
    blocks[16] = new Property("St. James Place" , "Orange",180,60, 0,305,105,370,16);
    blocks[17] = new GeneralBlock("Treasure",0,240,105,305,17);
    blocks[18] = new Property("Tennesse Avenue" , "Orange",180,60,0,175,105,240,18);
    blocks[19] = new Property("New York avenue" , "Orange",200,65,0,110,105,175,19);
    blocks[20] = new GeneralBlock("Parking",0,0,105,105,20);
    blocks[21] = new Property("Kentucky avenue" , "Red",220,75,105,0,170,105,21);
    blocks[22] = new Chance("Chance",170,0,235,105,22);
    blocks[23] = new Property("Indiana avenue" , "Red",220,75,235,0,300,105,23);
    blocks[24] = new Property("Illinois avenue" , "Red",240,80,300,0,365,105,24);   
    blocks[25] = new Company("Rail B & O" , 200 , 0,365,0,430,105,25);
    blocks[26] = new Property("Atlantic Avenue" , "Yellow",260,85,430,0,495,105,26);
    blocks[27] = new Property("Ventnor Avenue" , "Yellow",260,85,495,0,560,105,27);
    blocks[28] = new Company("Water Company" , 150 , 0,560,0,625,105,28);
    blocks[29] = new Property("Marvin Avenue" , "Yellow",280,90,625,0,690,105,29);
    blocks[30] = new GeneralBlock("GoToJail",690,0,799,105,30);
    blocks[31] = new Property("Pacific avenue" , "Green",300,100,695,105,799,170,31);
    blocks[32] = new Property("Carolina avenue" , "Green",300,100,695,170,799,235,32);
    blocks[33] = new GeneralBlock("Treasure",695,235,799,300,33);
    blocks[34] = new Property("Pannsylvania avenue" , "Green",320,105,695,300,799,365,34);
    blocks[35] = new Company("Rail Short" , 200 , 0,695,365,799,430,35);
    blocks[36] = new Chance("Chance",695,430,799,495,36);
    blocks[37] = new Property("Park avenue" , "Blue",350,115,695,495,799,560,37);
    blocks[38] = new GeneralBlock("Tax",695,560,799,625,38);
    blocks[39] = new Property("Koren avenue" , "Blue",400,135,695,625,799,690,39);
}

// Method to display the board details
void Board::displayBoard() const {
    for (int i = 0; i < size; ++i) {
        if (blocks[i] != nullptr) {
            std::cout << "Block " << i << ": ";
            blocks[i]->display();
        } else {
            std::cout << "Block " << i << ": Empty" << std::endl;
        }
    }
}

// Method to get a block at a specific position
Block* Board::getBlock(int index) const {
    if (index >= 0 && index < size) {
        return blocks[index];
    }
    return nullptr;
}

// Method to set a block at a specific position
void Board::setBlock(int index, Block* block) {
    if (index >= 0 && index < size) {
        blocks[index] = block;
    }
}

// Method to get the size of the board
int Board::getSize() const {
    return size;
}

vector<Block*> Board::getBlocks(){
    return blocks;
}

//Creates the SMFL window , adds Circles for houses , triangle instead of circles for Hotel and sqaure that is the player.
void Board::createGameWindow(int width, int height) {
    sf::RenderWindow window(sf::VideoMode(width, height), "Monopoly Board");

    // Load the Monopoly board image from file
    sf::Texture boardTexture;
    if (!boardTexture.loadFromFile("monopoly_board.png")) {
        std::cerr << "Error loading Monopoly board image!" << std::endl;
        return;
    }

    // Create a sprite to hold the board texture
    sf::Sprite boardSprite;
    boardSprite.setTexture(boardTexture);

    // Scale the image to fit the window
    sf::Vector2u textureSize = boardTexture.getSize();
    float scaleX = static_cast<float>(width) / textureSize.x;
    float scaleY = static_cast<float>(height) / textureSize.y;
    boardSprite.setScale(scaleX, scaleY);

    // Main game loop for rendering the board
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }

    window.clear(sf::Color::White);
    window.draw(boardSprite);
    
    // for all the blocks , if purchased mark the outline as the color of the player that bought.
    //if theres a house adds a circle (up to 4 circles)
    // if theres a hotel , removes circle and adds triangle.
    for (int i = 0; i < size; ++i) {
        if (blocks[i] != nullptr) {
            sf::RectangleShape rectangle(sf::Vector2f(
                blocks[i]->getx2() - blocks[i]->getx1(),
                blocks[i]->gety2() - blocks[i]->gety1()
            ));
            rectangle.setPosition(blocks[i]->getx1(), blocks[i]->gety1());
            rectangle.setFillColor(sf::Color::Transparent);
            rectangle.setOutlineColor(blockColors[i]);
            rectangle.setOutlineThickness(2);

            window.draw(rectangle);

            float radius = 5.0f;
            float margin = 5.0f;  // Margin to position circles inside the block
            if(blocks[i]->getType() == "Property"){
                Property* prop = dynamic_cast<Property*>(blocks[i]);
                int houses = prop->getHouses();
                // Create and position 3 circles inside the block
                if(prop->getHotels() == 0){
                    for (int j = 0; j < houses; ++j) {
                    sf::CircleShape circle(radius);
                    circle.setFillColor(stringToColor(prop->getOwner()));  // You can change the color as needed

                    // Position the circles evenly spaced within the block
                    float circleX = blocks[i]->getx1() + margin + j * (2 * radius + margin);
                    float circleY = blocks[i]->gety1() + (blocks[i]->gety2() - blocks[i]->gety1()) / 2 - radius;  // Center vertically
                    circle.setPosition(circleX, circleY);

                    window.draw(circle);  
                    }  
                }
                else{
                    sf::ConvexShape hotelTriangle;
                    hotelTriangle.setPointCount(3);  // A triangle has 3 points
                    float blockWidth = blocks[i]->getx2() - blocks[i]->getx1();
                    float blockHeight = blocks[i]->gety2() - blocks[i]->gety1();
    
                    // Define the points of the triangle to be centered within the block
                    float scaleFactor = 0.3f;
                    float triangleHeight = blockHeight * scaleFactor;
                    float triangleWidth = blockWidth * scaleFactor;
                    hotelTriangle.setPoint(0, sf::Vector2f(blocks[i]->getx1() + blockWidth / 2,blocks[i]->gety1() + (blockHeight - triangleHeight) / 2));  

                    hotelTriangle.setPoint(1, sf::Vector2f(blocks[i]->getx1() + (blockWidth - triangleWidth) / 2, blocks[i]->gety2() - (blockHeight - triangleHeight) / 2)); // Bottom-left

                    hotelTriangle.setPoint(2, sf::Vector2f(blocks[i]->getx2() - (blockWidth - triangleWidth) / 2,blocks[i]->gety2() - (blockHeight - triangleHeight) / 2)); // Bottom-right

                     // Set the fill color to the owner's color (or any color you prefer)
                    hotelTriangle.setFillColor(stringToColor(prop->getOwner()));


                    window.draw(hotelTriangle);
                }
                
            }
        }
    }
    //Iterates through all the players to show the current place of player.
    for (auto& player : players) {
            int playerPosition = player->getPlace();  // Get the player's current block index
            if (playerPosition >= 0 && playerPosition < size && blocks[playerPosition] != nullptr) {
                sf::RectangleShape playerSquare(sf::Vector2f(10.0f, 10.0f));  // Create a small square for the player
                playerSquare.setFillColor(stringToColor(player->getName()));  // Set the color based on the player's name

                // Position the player square inside the block
                float playerX = blocks[playerPosition]->getx1() + 5;  // Offset to position inside the block
                float playerY = blocks[playerPosition]->gety1() + 5;  // Offset to position inside the block
                playerSquare.setPosition(playerX, playerY);

                window.draw(playerSquare);  // Draw the player's token on the board
            }
        }

    window.display();
}

}
    //Sets the color of the Player as SFML color.
    sf::Color Board::stringToColor(const std::string& colorName) const{
    static const std::unordered_map<std::string, sf::Color> colorMap = {
        {"Red", sf::Color::Red},
        {"Green", sf::Color::Green},
        {"Blue", sf::Color::Blue},
        {"Black", sf::Color::Black},
        {"White", sf::Color::White},
        {"Yellow", sf::Color::Yellow},
        {"Cyan", sf::Color::Cyan},
        {"Magenta", sf::Color::Magenta},
        // Add more colors as needed
    };

    auto it = colorMap.find(colorName);
    if (it != colorMap.end()) {
        return it->second;
    } else {
        return sf::Color::Transparent; // Default color if not found
    }
}