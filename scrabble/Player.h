/*
* Player.h
*
*  Created on: Sep 18, 2016
*      Author: kempe
*/

/* Encodes the state of a player, mostly the name, tiles, points.
The main non-trivial ability of this class is to produce a vector
of Tile* from a string describing tiles to place/exchange. */

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


class Player
{
public:

	Player (std::string name, size_t maxTiles);

	~Player ();

	Tile* pop_back();


	int remaining_hand_total();

	std::string getLetters();

	std::set<Tile*> getHandTiles() const;

	bool hasTiles(std::string & move);

	std::vector<Tile*> takeTiles (std::string const & move);

	void addTiles (std::vector<Tile*> const & tilesToAdd);

	size_t getMaxTiles()const{return this->maxTiles;}
	std::string get_name()const{return this->name;}
	bool get_passed()const{return passed;}
	void set_passed(bool condition){passed = condition;}
	bool get_score()const{return score;}
	void adjust_score(int amount){score += amount;}

private:

	bool passed = false;
	int score = 0;
	std::vector<Tile*> hand;
	size_t maxTiles;
	std::string name;
};


#endif /* PLAYER_H_ */
