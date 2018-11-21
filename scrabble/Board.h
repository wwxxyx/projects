#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <set>
#include <vector>
#include <fstream>
#include <iostream>
#include <deque>
#include "Tile.h"
#include "Square.h"
#include "Move.h"
#include "Dictionary.h"

struct Board {

	/* [constructor & destructor] */
	Board(std::string filename);
	~Board();

	/* [methods] */
	std::deque<std::pair<std::string, size_t>> get_place_move_results(PlaceMove &m);

	/* returns a list of coordinates, in order, of each tile that will be placed
	on the board. Warning: must call all of the checks before calling this,
	or else the function could seg-fault */
	std::deque<Coord> get_proposal(PlaceMove& move);
	// attempts to throw exception to some of the more basic illegal moves
	void check_stupid(PlaceMove& move);
	// check if the first move is appropriately placed to cover the start tile
	bool check_first_move(PlaceMove& move);
	// check if any of the proposed coordinates would land out of bounds
	void check_out_of_bounds(Coord& curr);
	// looks at new words made, and verifies that they are in the dictionary
	bool check_connected(PlaceMove& m, std::deque<Coord> proposal);
	// check if a given square is already occupied on the board
	bool isOccupied(size_t y, size_t x);

	/* [data members] */
	std::vector<std::vector<Square*>> board;
	size_t rows;
	size_t columns;
	size_t x_origin;
	size_t y_origin;
};

#endif /* BOARD_H_ */
