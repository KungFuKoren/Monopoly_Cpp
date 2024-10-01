//kkorenn1@gmail.com

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <iostream>
#include <string>

using namespace std;

// Base Block class
class Block {
protected:
    string name;
    string type;
    int x1, y1, x2, y2; // New member variables for coordinates
    int place;
public:
    Block(const string& blockName, const string& blockType, int x1, int y1, int x2, int y2, int place);
    virtual ~Block();

    
    int getPlace();
    int getx1();
    int getx2();
    int gety1();
    int gety2();
    virtual void display() const;
    string getName() const; // Method to return the name of the block
    string getType() const;
};

// Derived class for Property
class Property : public Block {
private:
    int houses;
    int hotels;
    int price;
    int rent;
    string color;
    string owner = "Free";

public:
    Property(const string& blockName, const string& blockColor, int blockPrice, int blockRent, int x1, int y1, int x2, int y2, int place);    
    void addHouse();
    void addHotel();
    

    int getRent();
    int getPrice();
    int getHouses();
    int getHotels();
    string getOwner();
    void setOwner(string owner);
    string getColor();

    void display() const override;
};

// Derived class for Company
class Company : public Block {
private:
    int price;
    int rent;
    string owner = "Free";

public:
    Company(const string& blockName, int blockPrice, int blockRent, int x1, int y1, int x2, int y2, int place);  
    int getRent();
    int getPrice();
    string getOwner();
    void setOwner(string owner);
    string getColor(); // Companies don't have colors, will return an empty string

    void display() const override;
};

// Derived class for Chance
class Chance : public Block {
public:
    Chance(const string& blockName, int x1, int y1, int x2, int y2, int place);    
    void display() const override;
};

// General Block class (just name)
class GeneralBlock : public Block {
public:
    GeneralBlock(const string& blockName, int x1, int y1, int x2, int y2, int place);    
    void display() const override;
};

#endif // BLOCK_HPP
