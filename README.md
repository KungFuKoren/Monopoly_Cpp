//kkorenn1@gmail.com

My implementation of a Monopoly game.

The Objects - 
Player - simple constructor that can only be called for inside a game.

Blocks - different types of blocks according to the place.
every block gets Pixels that will be used to mark it , a placement number a Name and a Type.

Property - Has in addition Rent and Price - for buying and renting.
Company - has only price - because the amount to be payed differs.
Chance - only has a name - Treasure or Chance - for the special blocks.
GeneralBlock - For the four special squares ("Jail", "Go to jail", "Parking", "Go").

Board -
activates the SMFL visuals here - checks the properties and companies for changes - if it was purches or a realestate was bought.
displays all while the SMFL window runs.
each turn changes the place of the player according to player placement.
if the player was out of the game - removes the player from the player vector and from the SMFL.
then the properties are either transfered to another color if it was to a person and if owes money to bank it becomes transparent again.

Gameplay - 
Here creates the board and the player.
all of the game methods are inside it.
activates a Loop that runs while there isnt a player who won.
every turn moves the player according to the rules of the game
Included : *Jail methods , *Parking , *Chance and treasure cards , *Paying to a player, *Paying to bank, *Going Bankrupt, *Buying properties and companies.
game will end when theres only one player left in the Player vector or a Player has reached 4000$.
then terminates the program and releases the memory used.

Main - a simple main to Play the game.
test - Testing methods for the game and the important methods. including some edge cases.


To activate - 
make
make run_test for test
make run_main for main game.
make clean to clean.