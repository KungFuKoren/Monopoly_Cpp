//kkorenn1@gmail.com

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>


using namespace std;

class Player{
private:
    const string name;
    int money;
    int place = 0;
    int jailNum = 0;
    int GOOJ = 0;
    bool parking = false;
    bool doubleTimes = false;   
    sf::Color color;

public:
    Player(const string& name, const sf::Color& color);

    void transaction(int x);
    int getMoney();
    string getName();
    void move(int x);
    void setPlace(int x);
    int getPlace();
    void updateJail();
    void reset_jail();
    int getJail();
    void receivedGOOJcard();
    void usedGOOJcard();
    int getJailCards();
    void setParking();
    void resetParking();
    bool getParking();
    void setDouble();
    void resetDouble();
    bool getDouble();
    sf::Color getColor();
};




#endif // PLAYER_HPP