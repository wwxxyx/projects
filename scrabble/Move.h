#ifndef MOVE_H_
#define MOVE_H_

#include <vector>
#include <string>
#include <list>
#include <algorithm>
#include <sstream>
#include <stdexcept>
#include "Tile.h"
#include "Player.h"
#include "Bag.h"
#include "Board.h"
#include "Dictionary.h"
#include "Square.h"

// forward declaration to prevent circular includes
struct Board;

struct Move {

	/* Parses the given move m, and constructs the corresponding move subclass.
	m is assumed to be in the format described on the assignment sheet for a move command.
	The player passed in is used to verify that the proposed tiles are
	in fact in the player's hand.
	It can handle all three types of move (PASS, EXCHANGE, PLACE).
	Coordinates start with 1.
	*/

	Move * parseMove(std::string moveString, Player &p);

	/* Returns true iff the move is a PASS move */
	virtual bool isPass (){
		return false;
	}

	/* Returns true iff the move is an EXCHANGE move */
	virtual bool isExchange (){
		return false;
	}

	/* Returns true iff the move is a PLACE move */
	virtual bool isWord (){
		return false;
	}

	virtual void execute(Board & board, Bag & bag, Dictionary & dictionary) = 0;

	virtual ~Move();

	Player * _player;

	// constructor -- only accessible to subclasses
	Move(Player * p);

};

// Represents a pass move, where the player takes no action
struct PassMove : Move {

	/* Constructs a pass move. */
	PassMove(Player * p);

	/* Returns true iff the move is a PASS move */
	bool isPass (){
		return true;
	}

	void execute(Board & board, Bag & bag, Dictionary & dictionary);
};

// represents an exchange move, where a player replaces certain tiles
struct ExchangeMove : Move {

	/* Creates an EXCHANGE move, exchanging the tiles listed in the
	string (formatted according to the EXCHANGE command description)
	with new tiles from the bag.
	*/
	ExchangeMove(std::string tileString, Player * p);

	/* Returns true iff the move is an EXCHANGE move */
	bool isExchange (){
		return true;
	}

	void execute(Board & board, Bag & bag, Dictionary & dictionary);

	std::string str;
};

// represents a place move, where a player places one or more tiles onto the board.
struct PlaceMove : Move {


	/* Creates a PLACE move, starting at row y, column x, placing the tiles
	described by the string tileString. If "horizontal" is true, then the tiles
	are placed horizontally, otherwise vertically.
	Coordinates start with 1.
	The string m is in the format described in HW4; in particular, a '?'
	must be followed by the letter it is to be used as.
	*/
	PlaceMove (std::string entry, Player * p);

	/* Returns true iff the move is a PLACE move */
	bool isWord (){
		return true;
	}

	void execute(Board & board, Bag & bag, Dictionary & dictionary);

	void check_legality(Board & board, Dictionary & dictionary);

	// place a player's proposed tiles on the board
	void place_tiles(std::deque<Coord> proposal);
		// redact a player's attempted tile placement from the board if an error is detected during the evaluation
	void redact_tiles(std::deque<Coord> proposal);
	// looks at new words made, and verifies that they are in the dictionary
	void check_words_formed(std::deque<std::pair<std::string, size_t>> words_and_points){



	size_t x;
	size_t y;
	char direction;
	std::string str;
};
#endif /* MOVE_H_ */
