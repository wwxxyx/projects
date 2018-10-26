#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <set>
#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <iostream>
#include "Tile.h"
#include "Square.h"
#include "Move.h"
#include "Dictionary.h"

class Board{

public:
	/* constructor that initializes the board state with an empty board
	and the size and bonuses and start square based on the given file. */
	Board(std::string filename);
	~Board();
	/* Returns a pointer to the Square object representing the
	(y,x) position of the board. Indexing starts at 1 here.
	This is needed only to display the board. */
	Square * getSquare (size_t y, size_t x) const;
	std::vector<std::string> char_board;

	char getSquareChar(size_t y, size_t x);

	/* Returns the number of rows of the board.*/
	size_t getRows() const;

	/* Returns the number of columns of the board.*/
	size_t getColumns() const;

	bool isOccupied(size_t y, size_t x);

	void print_chars();

	std::vector<std::vector<Square*>> square;

	size_t rows;
	size_t columns;
	size_t start_x;
	size_t start_y;

};

#endif /* BOARD_H_ */
