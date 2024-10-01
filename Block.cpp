//kkorenn1@gmail.com

#include "Block.hpp"

// Block class implementation
Block::Block(const string& blockName, const string& blockType, int x1, int y1, int x2, int y2, int place) 
    : name(blockName), type(blockType), x1(x1), y1(y1), x2(x2), y2(y2), place(place) {}

Block::~Block() {}

void Block::display() const {
    cout << "Block Name: " << name << "\nType: "  << type << endl;
}
int Block::getPlace(){
    return place;
}
int Block::getx1(){
    return x1;
}
int Block::getx2(){
    return x2;
}
int Block::gety1(){
    return y1;
}
int Block::gety2(){
    return y2;
}

// Property class implementation
Property::Property(const string& blockName, const string& blockColor, int blockPrice, int blockRent, 
                   int x1, int y1, int x2, int y2, int place) 
    : Block(blockName, "Property", x1, y1, x2, y2, place), houses(0), hotels(0), price(blockPrice), rent(blockRent), color(blockColor) {}

void Property::addHouse() {
    houses++;
}

void Property::addHotel() {
    hotels++;
}

int Property::getRent() {
    return rent;
}

int Property::getPrice() {
    return price;
}
int Property::getHotels(){
    return hotels;
}

int Property::getHouses(){
    return houses;
}

string Property::getOwner() {
    return owner;
}

void Property::setOwner(string newOwner) {
    owner = newOwner;
}

string Property::getColor() {
    return color;
}

string Block::getType() const{
    return type;
}

void Property::display() const {
    Block::display();
    cout << "Houses: " << houses << "\nHotels: " << hotels << "\nPrice: $" << price
         << "\nRent: $" << rent << "\nColor: " << color << "\nOwner: " << owner<<"\n-------------\n" << endl;
}

// Company class implementation
Company::Company(const string& blockName, int blockPrice, int blockRent, int x1, int y1, int x2, int y2, int place) 
    : Block(blockName, "Company", x1, y1, x2, y2, place), price(blockPrice), rent(blockRent) {}

int Company::getRent() {
    return rent;
}

int Company::getPrice() {
    return price;
}

string Company::getOwner() {
    return owner;
}

void Company::setOwner(string newOwner) {
    owner = newOwner;
}

string Company::getColor() {
    return ""; // Companies don't have a color, so return an empty string.
}

void Company::display() const {
    Block::display();
    cout << "Price: $" << price << "\nRent: $" << rent << "\nOwner: "<< owner << endl;

}

// Chance class implementation
Chance::Chance(const string& blockName, int x1, int y1, int x2, int y2, int place) 
    : Block(blockName, "Chance", x1, y1, x2, y2, place) {}

void Chance::display() const {
    Block::display();
}

// GeneralBlock class implementation
GeneralBlock::GeneralBlock(const string& blockName, int x1, int y1, int x2, int y2, int place) 
    : Block(blockName, "General", x1, y1, x2, y2, place) {}

void GeneralBlock::display() const {
    Block::display();
}

// Method to return the name of the block
string Block::getName() const {
    return name;
}
