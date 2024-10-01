//kkorenn1@gmail.com
#include "Gameplay.hpp"

// Constructor to initialize the game with the given number of players
Gameplay::Gameplay(int playerCount) : numPlayers(playerCount) {
    if (playerCount < 2 || playerCount > 8) {
        throw invalid_argument("Player count must be between 2 and 8.");
    }

    // Create a default board (40 blocks)
    board = Board::getInstance();

    std::array<sf::Color, 8> colors = {
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Blue,
        sf::Color::Yellow,
        sf::Color::Magenta,
        sf::Color::Cyan,
        sf::Color::White,
        sf::Color::Black
    };

    std::array<std::string, 8> colorNames = {
        "Red",
        "Green",
        "Blue",
        "Yellow",
        "Magenta",
        "Cyan",
        "White",
        "Black"
    };

    // Create players and add them to the players list
    for (int i = 0; i < playerCount; ++i) { // Start from 0 for indexing
        string playerName = colorNames[i]; // Use color names instead of "Player" prefix
        players.push_back(new Player(playerName, colors[i]));
    }
    //Pushes Property and Company blocks into an array for later use , same for players.
    for (const auto& block : board->getBlocks()) {
        if (block->getType() == "Property") {
            properties.push_back(dynamic_cast<Property*>(block));
        }
        if (block->getType() == "Company") {
            companies.push_back(dynamic_cast<Company*>(block));
        }
    }
        for(const auto& player : players){
            board->addPlayer(player);
        }
    
}

// Destructor to clean up dynamically allocated memory
Gameplay::~Gameplay() {
    for (Player* player : players) {
        delete player;
    }
    players.clear();
}

//Starts the game with a number of Players between 2-8 and prints the Starting money sum.
void Gameplay::startGame() {
    cout << "Starting the Monopoly Game with " << numPlayers << " players!" << endl;

    // Display players' starting money
    for (Player* player : players) {
        cout << player->getName() << " starts with $" << player->getMoney() << endl;
       
    }

    // Create the board and open the game window in a different thread to run simultaneously 
    std::thread gameWindowThread(&Board::createGameWindow, Board::getInstance(40), 800, 800);
    gameWindowThread.detach(); 
}


// Display the current game state (players and board)
void Gameplay::displayGameState() const {
    cout << "Current Board State:" << endl;
    board->displayBoard();

    cout << "Player States:" << endl;
    for (Player* player : players) {
        cout << player->getName() << " has $" << player->getMoney() << endl;
    }
}

 // Rolls a random number between 1 , 6 and sets the cube to be such number 
void Gameplay::roll(){
    std::random_device rd; // Seed for the random number engine
    std::mt19937 gen(rd()); // Mersenne Twister engine seeded with random_device
    std::uniform_int_distribution<> distrib(1, 6); // Define the range [1, 6]

    cube1 = distrib(gen); 
    cube2 = distrib(gen);
    // std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // cube1 = (std::rand() % 6) + 1; 
    // cube2 = (std::rand() % 6) + 1; 
    // cout<<"Cube1:";
    // cin >> cube1;
    // cout<<"Cube2:";
    // cin >> cube2;
    cout<<"Cube 1 : "<<cube1<< " Cube 2 : "<<cube2<<endl;

}

//Gets the dice numbers and moves the player to the correct block. modulo board size to make sure we dont run out of board.
//in addition gives 200 if the next space moved is lower than current - means we passed through "Go" - does not apply for jail - diff method.
void Gameplay::move(Player* player){
    int temp = player->getPlace();
    int newPlace = (temp + cube1 + cube2)%(board->getSize());
    if(newPlace < temp){
        player->transaction(200);
    }
    player->setPlace(newPlace); // modulo of board size;
}

//Gives the Block the player stands on.
Block* Gameplay::getPlayerBlock(Player* player){
    int place = player->getPlace();
    Block* block = board->getBlock(place);
    return block;
}

//Method to know if we can purchase a block of type property or company - if its free and we have enough money.
bool Gameplay::canPurchase(Player* player){
    Block* currBlock = getPlayerBlock(player);
    string type = currBlock->getType();

    if(type == "Company" || type == "Property"){
        if(type == "Company"){
            Company* comp_block = dynamic_cast<Company*>(currBlock);
            if(comp_block->getOwner() == "Free" && comp_block->getPrice() <= player->getMoney()){
                return true;
            }
        }
        else{
            Property* comp_block = dynamic_cast<Property*>(currBlock);
            if(comp_block->getOwner() == "Free" && comp_block->getPrice() <= player->getMoney()){
                return true;
            }
        }
    }
    else{
        return false;
    }
    return false;
}

//uses the previous method to make sure if we can buy. then sets the owner to the player and use transaction to pay.
// works for block of type company or property only. (makes sure twice).
void Gameplay::purchase(Player* player){
    Block* block = getPlayerBlock(player);
    if(canPurchase(player)){
        if(block->getType() == "Company"){
            Company* comp_block = dynamic_cast<Company*>(block);
            comp_block->setOwner(player->getName());
            player->transaction(-(comp_block->getPrice()));
            cout<<player->getName()<<" bought the Company: "<<block->getName()<<endl;
            cout<<player->getName()<<" balance: "<<player->getMoney()<<"$"<<endl;
            board->setBlockOutlineColor(player->getPlace(), player->getColor());
        }
        else{
            Property* comp_block = dynamic_cast<Property*>(block);
            comp_block->setOwner(player->getName());
            player->transaction(-(comp_block->getPrice()));
            cout<<player->getName()<<" bought the Property: "<<block->getName()<<endl;
            cout<<player->getName()<<" balance: "<<player->getMoney()<<"$"<<endl;
            board->setBlockOutlineColor(player->getPlace(), player->getColor());

        }
    }
    else{
        cout<<"Cannot be purchased at this turn"<<endl;
    }
}

//Boolean method to know if we can build a house on the block were standing on.
// checks if we are on  Comp / Prop block , have enough money , doesnt have 4 houses already and if we own all of the street.
// lastly , checks if this current block has more houses than the rest of the street.
bool Gameplay::canBuildHouse(Player* player){
    vector<Property*> prop_samecolor; 
    Block* block = getPlayerBlock(player);
    if(block->getType() != "Property"){
        cout << "Not a property" << endl;
        return false;
    }
    Property* curr = dynamic_cast<Property*>(block);
    string color = curr->getColor();
    if(curr->getPrice() > player->getMoney()){
        cout << "Insufficent funds" << endl;
        return false;
    }
    for(const auto& prop : properties){
        if(prop->getColor() == color){
            prop_samecolor.push_back(prop);
        }
    }
    // false if theres already 4 or not owns the whole street
    if(curr->getHouses() == 4 || (ownsAllStreet(player, prop_samecolor) == false)){
        return false;
    }
    for(const auto& prop : prop_samecolor ){
        if(curr->getHouses() > prop->getHouses()){
            cout<<"You got more houses than the rest of the street. Can't buy another"<<endl;
            return false;
        }
    }
    return true;
    

}


//Same method to make sure if we can build a house but this time uses property and player.
bool Gameplay::canBuildHouse(Property* prop , Player* player){
    vector<Property*> prop_samecolor;
    if(prop->getOwner() != player->getName()){
        return false;
    }
    if(prop->getPrice() > player->getMoney()){
        return false;
    }
    if(prop->getHouses() >= 4){
        return false;
    }
    for(const auto& property : properties){
        if(property->getColor() == prop->getColor()){
            prop_samecolor.push_back(property);
        }
    }
    if(ownsAllStreet(player , prop_samecolor) == false){
        return false;
    }
    for(const auto& proper : prop_samecolor){
        if(prop->getHouses() > proper->getHouses()){
            return false;
        }
    }
    return true;

}

//Checks if player owns all the street.
bool Gameplay::ownsAllStreet(Player* player ,vector<Property*> prop_samecolor){
    for(const auto& prop : prop_samecolor){
        if(prop->getOwner() != player->getName()){
            return false;
        }
    }
    return true;
}




//Builds house , if we can build house - updates the property number of houses and transaction to pay.
void Gameplay::buildHouse(Player* player){
    Block* block = getPlayerBlock(player);
    if(canBuildHouse(player)){
        Property* curr = dynamic_cast<Property*>(block);
        curr->addHouse();
        player->transaction(-(curr->getPrice()));
        cout<<player->getName()<<" bought a house in " <<block->getName()<<endl;
        cout<<"There are " <<curr->getHouses()<<" Houses in this property"<<endl;
        cout<<player->getName()<<" balance: "<<player->getMoney()<<"$"<<endl;
    }
}

// same as above just with Property.
void Gameplay::buildHouse(Property* prop, Player* player){
    if(canBuildHouse(prop, player)){
        prop->addHouse();
        player->transaction(-(prop->getPrice()));
        cout<<player->getName()<<" bought a house in " <<prop->getName()<<endl;
        cout<<"There are " <<prop->getHouses()<<" Houses in this property"<<endl;
        cout<<player->getName()<<" balance: "<<player->getMoney()<<"$"<<endl;
    }
}

//Checks if we can build hotel on a certain property. only if we have 4 houses at the same location and 0 hotels.
// 4 houses because its the max amout of houses can be built. (made sure in build house).
bool Gameplay::canBuildHotel(Player* player){
    Block* block = getPlayerBlock(player);
    vector<Property*> prop_samecolor; 
    

    if(block->getType() == "Property"){
        Property* prop = dynamic_cast<Property*>(block);
        for(const auto& curr : properties){
            if(curr->getColor() == prop->getColor()){
                prop_samecolor.push_back(curr);
            }
        }
        for(const auto& proper : prop_samecolor){
            if(proper->getHouses() != 4){
                return false;
            }
        }
        if(prop->getHotels() > 0){
            return false;
        }
        if(prop->getHouses() == 4){
            if((prop->getPrice()*4 + 100)  <= player->getMoney()){
                cout<<"Can Build hotel"<<endl;
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

//Same method as above just with Property.
bool Gameplay::canBuildHotel(Property* prop, Player* player){
    vector<Property*> prop_samecolor; 
    for(const auto& curr : properties){
        if(curr->getColor() == prop->getColor()){
            prop_samecolor.push_back(curr);
        }
    }
    for(const auto& curr : prop_samecolor){
        if(curr->getHouses() != 4){
            return false;
        }
    }
    if(prop->getHotels() > 0){
            cout << "Can only build one hotel per Property" << endl; 
            return false;
        }
    if(prop->getHouses() == 4){
            if((prop->getPrice()*4 + 100)  <= player->getMoney()){
                cout<<"Can Build !"<<endl;
                return true;
            }
            else{
                cout << "Insufficient funds" << endl;
                return false;
            }
        }    
    return false;
}

//Building hotel and pays for it. updates the property and players money.
void Gameplay::buildHotel(Player* player){
    Block* block = getPlayerBlock(player);
    if(block->getType() == "Property"){
        Property* prop = dynamic_cast<Property*>(block);
            player->transaction(-(prop->getPrice()*4 + 100));
            prop->addHotel();
            cout<<player->getName()<<" bought a hotel in" <<block->getName()<<endl;
            cout<<player->getName()<<" balance: "<<player->getMoney()<<"$"<<endl;
        
    }
    
}

//Same as above only with Property.
void Gameplay::buildHotel(Property* prop, Player* player){
        player->transaction(-(prop->getPrice()*4 + 100));
        prop->addHotel();
        cout<<player->getName()<<" bought a hotel in" <<prop->getName()<<endl;
        cout<<player->getName()<<" balance: "<<player->getMoney()<<"$"<<endl;
    
}

//Pays to a certain player amount. if a player lacks the money to pay - will transfer all of its properties.
void Gameplay::payToPlayer(Player* from , Player* to , int amount){
   int money = from->getMoney();
    if(money < amount){
        for(const auto& prop : properties){
            if(prop->getOwner() == from->getName()){
                prop->setOwner(to->getName());
                board->setBlockOutlineColor(prop->getPlace(), to->getColor());

            }
        }
         for(const auto& comp : companies){
            if(comp->getOwner() == from->getName()){
                comp->setOwner(to->getName());
                board->setBlockOutlineColor(comp->getPlace(), to->getColor());

            }
        }
        from->transaction(-(money));
        to->transaction(money);
        cout<<from->getName()<<" Paid "<<to->getName()<<" "<<amount<<"$"<<endl;
        cout<<from->getName()<<" balance:"<<from->getMoney()<<endl; 
        cout<<to->getName()<<" balance:"<<to->getMoney()<<endl;
        cout<<from->getName()<<" has transfered all his real-estate to "<<to->getName()<<endl; 
        from->transaction(-10000);
    }
    else{
        from->transaction(-(amount));
        to->transaction(amount);
        cout<<from->getName()<<" Paid "<<to->getName()<<" "<<amount<<"$"<<endl;
        cout<<from->getName()<<" balance:"<<from->getMoney()<<endl; 
        cout<<to->getName()<<" balance:"<<to->getMoney()<<endl; 
    }
}

//Declare bankruptcy and removes player from the Players vector. if player still has properties - reset them and be available to purchase.
//Sends out a message of bankrupt.
void Gameplay::declareBankruptcy(Player* player) {
    string toRemove = player->getName();
    // Update ownership of properties
    for (const auto& prop : properties) {
        if (prop->getOwner() == toRemove) {
            prop->setOwner("Free");
            board->setBlockOutlineColor(prop->getPlace(), sf::Color::Transparent);
        }
    }

    // Update ownership of companies
    for (const auto& comp : companies) {
        if (comp->getOwner() == toRemove) {
            comp->setOwner("Free");
            board->setBlockOutlineColor(comp->getPlace(), sf::Color::Transparent);
        }
    }

    // Remove the player from the vector by name and delete the player object
    auto it = remove_if(players.begin(), players.end(),
        [&toRemove](Player* p) {
            return (p->getName() == toRemove);
        });

    // Delete the players that are being removed
    
    for (auto iter = players.begin(); iter != it; ++iter) {
        if ((*iter)->getName() == toRemove) {
            delete *iter; // Free memory for the player object
            *iter = nullptr;
        }
    }

    // Erase the removed players from the vector
    players.erase(it, players.end());

    // Notify about bankruptcy
    cout << toRemove + " Declared BANKRUPTCYYYYY and left the game" << endl;
    cout << "Remaining Players: " << endl;
    for (const auto& play : players) {
        cout << play->getName() << "\n" << endl;
    }
     
}







//Main method , checks on which block the player is and sets what to do according to the block type;
//Block type - Property - checks if can purchase or can build realestate if the block is free , if occupied - calls for gottaPay to pay the owner.
//Block type Company - checks if can purchase if free , if occupied gottaPay- pays for the owner.
//Block type Chance - gets chance or treasure according to the block name.
//Block type General - parking - skips turn , Jail (only active if player.jailNum is not  0) if not then nothing, if goToJail - sends to jail and update jailnum.
//if Jail is activated - asks the plater to roll to see if theres a double , by activating tryToLeaveJail 
// 3 methods - roll basic without asking, if has a card - asks if want to use , lastly asks if want to pay 50$. after 3 turns automatically paying 50$ if doesnt have - DeclareBankruptcy

void Gameplay::landedOnBlock(Player* player){
    //Actions if on Property
    Block* block = getPlayerBlock(player);
    block->display();
    if(block->getType() == "Property"){
        Property* prop = dynamic_cast<Property*>(block);
        if(prop->getOwner() == "Free"){
            if(canPurchase(player)){
                cout<<"Do you want to purchase " << prop->getName() << " ?"<<endl;
                if(getYesOrNoInput()){
                    purchase(player);
                }
                return;
            }
        }
        if(prop->getOwner() == player->getName()){
            if(canBuildHouse(player)){
                cout<<"Want to build a House on " << prop->getName()<< "?" << endl;
                if(getYesOrNoInput()){
                    buildHouse(player);
                }
                return;

            }
            if(canBuildHotel(player)){
                cout<<"Want to build a Hotel on " << prop->getName()<< "?" << endl;
                if(getYesOrNoInput()){
                    buildHotel(player);
                }
                return;
            }
        }
        else{
            Player* owner = getBlockPlayer(block);
            if(owner == nullptr){
                return;
            }
            cout<<"You dont own this Property , Gotta pay !" <<endl;
            gottaPay(player, prop, owner);
            return;
        }
    }
    //Actions if on Company
    if(block->getType() == "Company"){
        Company* comp = dynamic_cast<Company*>(block);
        if(comp->getOwner() == "Free"){
            if(canPurchase(player)){
                cout<<"Do you want to purchase " << comp->getName() << " ?" << endl;
                if(getYesOrNoInput()){
                    purchase(player);
                }
                return;
            }
        }
        if(comp->getOwner() == player->getName()){
            return;
            //do nothing
        }
        else{
            Player* owner = getBlockPlayer(block);
            if(owner == nullptr){
                return;
            }
            cout<<"You dont own this Company , Gotta pay !" <<endl;
            gottaPay(player, comp, owner);
            return;
        }
    }
    //Rest of the actions
    if(block->getName() == "Chance"){
        getChance(player);
        return;
    }
    if(block->getName() == "GoToJail"){
        setJail(player);
        return;
    }
    if(block->getName() == "Parking"){
        player->setParking();
        return;
    }
    if(block->getName() == "Jail" && player->getJail() > 0){
       tryToLeaveJail(player);
       return;
    }
    if(block->getName() == "Tax"){
        player->transaction(-100);
        cout<<"Landed on Tax block. Pay a fine of 100$"<<endl;
        return;
    }
    if(block->getName() == "Treasure"){
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(1,500);
        int rand = distr(gen);
        player->transaction(rand);
        cout << "Landed on Community chest Reward : "<< rand<<"$"<<endl;
        return;

    }

    return;
}

//Retirms the player standing on the block.
Player* Gameplay::getBlockPlayer(Block* block) {
    std::string owner = "Free";

    // Check if block is a Property and get the owner
    if (block->getType() == "Property") {
        Property* prop = dynamic_cast<Property*>(block);
        if (prop) {
            owner = prop->getOwner();
        }
    }

    // Check if block is a Company and get the owner
    if (block->getType() == "Company") {
        Company* comp = dynamic_cast<Company*>(block);
        if (comp) {
            owner = comp->getOwner();
        }
    }
    
    if (owner == "Free") {
        std::cout << "No player owns this block." << std::endl;
        return nullptr;
    }

    // Search for the player by owner name
    for (const auto& player : players) {
        if (player->getName() == owner) {
            return player; // Return the player if found
        }
    }

    std::cout << "Player with name '" << owner << "' not found." << std::endl;
    return nullptr; // Return nullptr if the player is not found
}

//Pays to a player on a certain block by the number of houses and hotels.
//calls payToPlayer after the said amount is calculated.

void Gameplay::gottaPay(Player* player , Property* prop , Player* owner){
    int houses = prop->getHouses();
    int hotels = prop->getHotels();
    int rent = prop->getRent();
    if(hotels > 0){
        payToPlayer(player, owner , (rent*pow(2,5)));
    }
    else{
        payToPlayer(player, owner , (rent*(pow(2,houses))));
    }
}
//Same as above but for company
void Gameplay::gottaPay(Player* player , Company* comp , Player* owner){
    string name = comp->getName();
    if(name.find("Rail") == 0){
        int howMany = 0;
        for(const auto& company : companies){
            if(company->getOwner() == owner->getName() && company->getName().find("Rail") == 0){
                howMany ++;
            }
        }
        payToPlayer(player, owner, 50*howMany);
    }
    else{
        int sum = cube1 + cube2;
        payToPlayer(player, owner, sum*10);
    }
}    


//Sets the jailnum to know player is in jail.
void Gameplay::setJail(Player* player){
    player->updateJail();
    goToJail(player);
    cout<<"You are in Jail !"<<endl;
}

//sends player to jail without going thorugh "Go"
// calls for the method above.
void Gameplay::goToJail(Player* player){
    int jailPlace = 10; // Need to be changed for other sized board
    player->setPlace(jailPlace);    
}
//Sets jailNum to 0 to indicate leaving jail.
void resetJail(Player* player){
    player->reset_jail();
}
//Returns players playing
vector<Player*> Gameplay::getPlayers(){
    return players;
}

//simple yes or no input
bool Gameplay::getYesOrNoInput() {
    char input;
    while (true) {
        cout << "Enter (y/n): ";
        cin >> input;

        // Check if input is 'y', 'Y', 'n', or 'N'
        if (input == 'y' || input == 'Y') {
            return true;
        } else if (input == 'n' || input == 'N') {
            return false;
        } else {
            cout << "Invalid input. Please enter 'y' or 'n'." << endl;
        }
    }
}
//if players jail num is less than 3 - can tryr and leave jail without paying , after the third time forcing to pay.
// if theres a get out of jail card asks if want to use and lastly asks to pay 50$
void Gameplay::tryToLeaveJail(Player* player){
    if(player->getJail() < 3){
        cout<<"Choose method to leave Jail"<<endl;
        if(cube1 == cube2){
            cout<<"Try and roll two more times"<<endl;
            roll();
            if(cube1 == cube2){
                roll();
                if(cube1 == cube2){
                    cout<<"Was able to leave jail with 3 dice rolls ! LUCKY !"<<endl;
                    return;
                }
            }
        }
        player->updateJail();
        if(getOutOfJailCard(player)){
            cout<<"You can use your get out of jail free card"<<endl;
            if(getYesOrNoInput()){
                player->usedGOOJcard();
                resetJail(player);
                cout<<"You got out of jail by using your card !"<<endl;
                return;
            }
        }
        if(player->getMoney() >= 50){
            cout<<"Can leave jail by paying 50$"<<endl;
            if(getYesOrNoInput()){
                player->transaction(-50);
                resetJail(player);
                cout<<"Left jail and paid 50$"<<endl;
                return;
            }
        }
    }
    else{
        if(player->getMoney() < 50){
            player->transaction(-4005);
            return;
        }
        else{
            player->transaction(-50);
            resetJail(player);
            cout<<"Paid 50$ and got out of jail"<<endl;
            return;
        }
    }
}

//Helper for the above method if theres a card for the player.
bool Gameplay::getOutOfJailCard(Player* player){
    if(player->getJailCards() > 0){
        return true;
    }
    else{return false;}
}

//Checks if only one player remains in the game or if a certain player reached 4000$
bool Gameplay::CheckWin(){
    if(players.size() == 1){
        Player* name = players[0];
        cout<<"Congratulations "<<name->getName()<<" won the game !! - he was the only one left"<<endl;
        return true;

    }
    for(const auto& player : players){
        if(player->getMoney() >= 4000){
            cout<<"Congratulations "<<player->getName()<<" won the game !! - reached 4000$"<<endl;
            return true;
        }
    }
    return false;
}


//Turn method - being called every turn obviously :)
// prints the player playing and sees that hes not in jail , if not calls for landedOnBlock and if the player still has money prints the available options.
//if player isnt in jail moves gim , if theres a double three times puts player in jail.
//if a player is out of money after the round , checks declare bankruptcy and removes the player.
//

void Gameplay::Turn(Player* player){
    cout<<"---------------"<<endl;
    cout<<player->getName()<<" Is playing"<<endl;
    roll();
    if(player->getJail() == 0){
        move(player);
    }
    if(player->getDouble() && cube1 == cube2){
        setJail(player);
        cout<<player->getName()<<" rolled double 3 times and now in Jail !"<<endl;
        return;
    }
    landedOnBlock(player);
    if(player->getMoney() >= 0){
        cout<<"Printing Available moves"<<endl;
        if(player->getJail() == 0 && !(player->getParking())){
            whatCanIDo(player);}
        if(player->getParking()){player->resetParking();}
    }
    else{
        declareBankruptcy(player);
        board->removePlayer(player->getName());
    }


          
    
   
}
//iterates through the players verctor and returns next player.
Player* Gameplay::getNextPlayer() {
    if (players.empty()) {
        return nullptr; 
    }
    turn = (turn + 1) % players.size(); 
    return players[turn];
}

//run game is the main game loop , works while no one have won the game.
//Calls for turn few times if we got a double - third time moves the player to jail
void Gameplay::runGame(){
    
    while(!CheckWin()){
        Player* player = getNextPlayer();
        Turn(player);
        if(cube1 == cube2){
            Turn(player);
            if(cube1 == cube2){
                player->setDouble();
                Turn(player);
            }
        }
        if(player->getDouble()){
            player->resetDouble();
        }
    }

}
//Resets jail.
void Gameplay::resetJail(Player* player){
    player->reset_jail();
}

//Iterates through all of the properties to check what can be done - thats why we have the methods to buy houses not only when player stands on the block.
//goes throguh all companies and properties
void Gameplay::whatCanIDo(Player* player){
    for(auto& prop : properties){
        if(prop->getOwner() == player->getName()){
            if(canBuildHouse(prop, player)){
                cout<<"Do you want to build a house in "<<prop->getName()<<"?"<<endl;
                if(getYesOrNoInput()){
                    buildHouse(prop, player);
                }
            }
            if(canBuildHotel(prop, player)){
                cout<<"Do you want to build a hotel in "<<prop->getName()<<"?"<<endl;
                    if(getYesOrNoInput()){
                        buildHotel(prop, player);
                } 
            }
        }
    }
}
//Gets a random number between 1 and 10 
// 1~ 3 - gives a random Company or property for player to go to , then activates landed on block.
//if random place is jail or go to jail - player goes to jail.
//4~6 - 4- pays each player an amount of 1~25
// 5 - Pays amount to bank.
// 6 - Repairs for house and hotels 
// if doesnt have enough money calls for bankrupt and returns.
// 7 - go to nearest company
// 8 - go to "Go" and receive 200$
// 9 - goes back few blocks.
// 10- Gives Get out of jail free card.
void Gameplay::getChance(Player* player){
    std::random_device rd;   // Random seed
    std::mt19937 gen(rd());  // Mersenne Twister engine

    // Uniform distribution for random numbers between 1 and 10
    std::uniform_int_distribution<> chanceDist(1, 10);
    int random = chanceDist(gen);

    cout << "________\n" << random << "-----" << endl;

    if(random < 4){
        int randomGoTo = (rand()%39);
        int curr = player->getPlace();
        if(randomGoTo == 10 || randomGoTo == 30){
            cout<<"Chance drawn : Go to Jail - Unlucky !"<<endl;
            landedOnBlock(player);
            return;
        }
        if(board->getBlocks()[randomGoTo]->getType() == "Chance" || board->getBlocks()[randomGoTo]->getType() == "GeneralBlock"){
            randomGoTo ++;
        }
        else{
            string name = board->getBlocks()[randomGoTo]->getName();
            cout<<"Go to "<<name<<endl;
            if(curr < randomGoTo){
                player->transaction(200);
                cout<<"Moved through Go , got 200$"<<endl;
            }
            player->setPlace(randomGoTo);
            landedOnBlock(player);
            return;
        }

    }
    else if(random == 4 || random == 5 || random == 6){
        if(random == 4){
            int amount = (rand()%25);
            cout<<"Pay each Player "<<amount<<"$"<<endl;
            for(const auto& play : players){
                if(play->getName() != player->getName()){
                    payToPlayer(player, play,amount);
                }
            }
            return;
        }
        if(random == 5){
            int amount = (rand()%50);
            cout<<"You have a debt to bank of "<<amount<<"$"<<endl;
            if(player->getMoney() > amount){
                player->transaction(-amount);
                return;
            }
            else{
                player->transaction(-4005);
                return;
            }
        }
        if(random == 6){
            cout<<"Repairs are due - for each house pay 25$ , for each hotel 100$"<<endl;
            int playerHotels = 0;
            int playerHouses = 0;
            for(const auto& prop : properties){
                if(prop->getOwner() == player->getName()){
                    if(prop->getHotels() > 0){
                        playerHotels++;
                    }
                    if(prop->getHotels() == 0){
                        playerHouses += prop->getHouses();
                    }
                }
            }
            int sum = playerHotels*100 + playerHouses*25;
            if(player->getMoney() < sum){
                cout<<"insufficient funds"<<endl;
                player->transaction(-4005);
                return;
            }
            else{
                cout<<"Paid "<<sum<<" to the bank"<<endl;
                player->transaction(-sum);
                return;
            }
        }
    }
    else if(random == 7 || random == 8 || random == 9){
            if(random == 7){
                cout<<"Advance to the nearest Company"<<endl;
                int curr = player->getPlace();
                for(int i = curr ; i < 40 ; i++){
                    int toGo = (curr+i)%40;
                    if(board->getBlocks()[toGo]->getType() == "Company"){
                        player->setPlace(toGo);
                        cout<<"Youre in "<<board->getBlocks()[toGo]->getName()<<endl;
                        landedOnBlock(player);
                        return;
                    }
                }
            }
            if(random == 8){
                cout<<"Go to Go and get 200$"<<endl;
                player->setPlace(0);
                player->transaction(200);
                landedOnBlock(player);
                return;
            }
            if(random == 9){
                int randomaly = (rand()%5);
                cout<<"Go back "<<randomaly<<" blocks"<<endl;
                int newPlace = (40+player->getPlace()-randomaly)%40;
                player->setPlace(newPlace);
                landedOnBlock(player);
                return;
            }
    }
    else{
        cout<<"Got a get out of jail free card"<<endl;
        player->receivedGOOJcard();
        return;
    }
}
//Returns board.
Board* Gameplay::getBoard(){
    return board;
}
//Method for me :)
void Gameplay::setCubes(int x, int y){
    cube1 = x;
    cube2 = y;
}