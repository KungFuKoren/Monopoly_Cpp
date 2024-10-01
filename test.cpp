//kkorenn1@gmail.com

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Gameplay.hpp"  // Include your Gameplay class header
#include "Player.hpp"    // Include your Player class header
#include "Block.hpp"  

TEST_CASE("Player Management") {
    Gameplay game(3);
    
    
    // Check that the players were added correctly
    CHECK(game.getPlayers().size() == 3);
    CHECK(game.getPlayers()[0]->getName() == "Red");
    CHECK(game.getPlayers()[1]->getName() == "Green");
    CHECK(game.getPlayers()[2]->getName() == "Blue");
}

TEST_CASE("Player Bankruptcy") {
    Gameplay game(3);
    // Simulate bankruptcy
    game.declareBankruptcy(game.getPlayers()[0]);
    CHECK(game.getPlayers().size() == 2);
    game.declareBankruptcy(game.getPlayers()[0]);
    CHECK(game.getPlayers().size() == 1);
}

TEST_CASE("Can Purchase") {
    Gameplay game(3);
    game.setCubes(1,2);
    game.move(game.getPlayers()[0]);
    bool canPurch = game.canPurchase(game.getPlayers()[0]);
    bool canHouse = game.canBuildHouse(game.getPlayers()[0]);
    bool canHotel = game.canBuildHotel(game.getPlayers()[0]);
    CHECK(canPurch == true);
    CHECK(canHouse == false);
    CHECK(canHotel == false);
}

TEST_CASE("Can buy House and hotel") {
    Gameplay game(3);
    game.setCubes(1,0);
    game.move(game.getPlayers()[0]);
    game.purchase(game.getPlayers()[0]);
    game.setCubes(1,1);   
    game.move(game.getPlayers()[0]);
    game.purchase(game.getPlayers()[0]);
    Property* prop1 = dynamic_cast<Property*>(game.getBoard()->getBlocks()[1]);
    Property* prop2 = dynamic_cast<Property*>(game.getBoard()->getBlocks()[3]);

    bool canHouse = game.canBuildHouse(game.getPlayers()[0]);
    //Checks if the property owner has changed

    CHECK(prop1->getOwner() == game.getPlayers()[0]->getName());
    CHECK(prop2->getOwner() == game.getPlayers()[0]->getName());

    //cheks if can buy first house - can (ownes all street)
    CHECK(canHouse == true);
    game.buildHouse(game.getPlayers()[0]);
    bool canHouse2 = game.canBuildHouse(game.getPlayers()[0]);
    //checks if cant buy house - more houses in this block than the rest of the street
    CHECK(canHouse2 == false);

    bool canHotel1 = game.canBuildHotel(game.getPlayers()[0]);
    //Checks if can build a hotel - false because not enough houses
    CHECK(canHotel1 == false);
    prop1->addHouse();
    game.gottaPay(game.getPlayers()[1], prop1, game.getPlayers()[0]);

    prop1->addHouse();
    prop1->addHouse();
    prop1->addHouse();

    prop2->addHouse();
    prop2->addHouse();
    prop2->addHouse();
     bool canHotel2 = game.canBuildHotel(game.getPlayers()[0]);
    //Checks if can build a hotel true after adding 4 houses to each block
    CHECK(canHotel2 == true);
    game.buildHotel(game.getPlayers()[0]);
    CHECK(prop2->getHotels() ==1);
    bool canHotel3 = game.canBuildHotel(game.getPlayers()[0]);
    CHECK(canHotel3 == false);

    
}

TEST_CASE("Paying"){
    Gameplay game1(3);
    game1.setCubes(2,4);
    game1.move(game1.getPlayers()[1]);
    game1.purchase(game1.getPlayers()[1]);
    game1.setCubes(1,1);
    game1.move(game1.getPlayers()[1]);
    game1.purchase(game1.getPlayers()[1]);
    game1.setCubes(0,1);
    game1.move(game1.getPlayers()[1]);
    game1.purchase(game1.getPlayers()[1]);
    Property* prop3 = dynamic_cast<Property*>(game1.getBoard()->getBlock(6));
    int currMoney1 = game1.getPlayers()[1]->getMoney();
    game1.gottaPay(game1.getPlayers()[2], prop3, game1.getPlayers()[1]);
    int to_pay = prop3->getRent()*(pow(2,prop3->getHouses()));
    int currMoney2 = game1.getPlayers()[1]->getMoney();
    //Checks paying method with no houses.
    CHECK( currMoney2 == currMoney1 + to_pay);
    prop3->addHouse();
    game1.gottaPay(game1.getPlayers()[2], prop3, game1.getPlayers()[1]);
    int to_pay2 = prop3->getRent()*(pow(2,prop3->getHouses()));
    int currMoney3 = game1.getPlayers()[1]->getMoney();
    //Checks paying after 1 house
    CHECK(currMoney3 == to_pay2+currMoney2);
    prop3->addHotel();
    game1.gottaPay(game1.getPlayers()[2], prop3, game1.getPlayers()[1]);
    int to_pay3 = prop3->getRent()*(pow(2,5));
    int currMoney4 = game1.getPlayers()[1]->getMoney();
    //Checks paying after 1 hotel
    CHECK(currMoney4 == to_pay3+currMoney3);


}

TEST_CASE("End game 4000$") {
    Gameplay game2(3);
    game2.getPlayers()[2]->transaction(4000);
    CHECK(game2.CheckWin() == true);
    
   
}

TEST_CASE("End game no one left") {
    Gameplay game2(3);
    game2.declareBankruptcy(game2.getPlayers()[0]);
    game2.declareBankruptcy(game2.getPlayers()[1]);

    CHECK(game2.CheckWin() == true);
    
   
}
