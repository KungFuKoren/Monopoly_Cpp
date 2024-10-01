//kkorenn1@gmail.com

#include "Player.hpp"

// Constructor to initialize player with a name and default starting money
//simple player class with no deletion - the deletion is in the game method.
Player::Player(const std::string& playerName, const sf::Color& color) 
    : name(playerName), money(1500), color(color), place(0), jailNum(0), GOOJ(0), parking(false), doubleTimes(false) {
}


void Player::transaction(int x) {
    money += x;  
}

int Player::getMoney() {
    return money;
}

string Player::getName() {
    return name;
}

void Player::setPlace(int x){
    place = x;
}

int Player::getPlace(){
    return place;
}

void Player::updateJail(){
    jailNum++;
}

void Player::reset_jail(){
    jailNum = 0;
}
int Player::getJail(){
    return jailNum;
}
void Player::receivedGOOJcard(){
    GOOJ++;
}
void Player::usedGOOJcard(){
    if(GOOJ > 0){
        GOOJ--;
    }
    else{
        cout<<"BUG NEGATIVE CARDS"<<endl;
    }
}
int Player::getJailCards(){
    return GOOJ;
}
void Player::setParking(){
    parking = true;
}
void Player::resetParking(){
    parking = false;
}
bool Player::getParking(){
    return parking;
}
void Player::setDouble(){
    doubleTimes = true;
}
void Player::resetDouble(){
    doubleTimes = false;
}
bool Player::getDouble(){
    return doubleTimes;
}

sf::Color Player::getColor(){
    return color;
}