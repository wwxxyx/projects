#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <set>
#include <vector>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <locale>
#include "Tile.h"


struct Player {

	/* [constructor & destructor] */
	Player (std::string name, size_t maxTiles);
	~Player ();

	/* [methods] */

	/* return the remaining points left in a player's hand at end of game
	and deletes those tiles from the player's hand */
	int remaining_hand_total_points();
	// return the set of tiles that a player currently holds
	std::set<Tile*> getHandTiles();
	// check if a player has the tiles in "move"
	bool has_tiles(std::string & move);
	// take away the tiles in "move" from the player
	std::vector<Tile*> take_tiles(std::string & move);
	// add all the tiles in the vector to the player's hand.
	void add_tiles(std::vector<Tile*> & tiles_to_add);
	/* [data members] */
	std::string name;
	size_t max_tiles;
	bool passed;
	size_t score;
	std::vector<Tile*> hand;
};


#endif /* PLAYER_H_ */
