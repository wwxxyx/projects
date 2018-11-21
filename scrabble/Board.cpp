#include "Board.h"
#include "Dictionary.h"
#include <stdexcept>


/* constructor: initializes the board as an empty board
w/ a given size, allocates bonuses for squares based on the config file. */
Board::Board(std::string filename){
	std::ifstream ifile(filename);
	std::string str1;
	ifile >> columns >> rows >> x_origin >> y_origin;
	std::getline(ifile,str1);
	board.resize(rows,std::vector<Square*>(columns,0));
	for(size_t i=0; i<rows; i++){
		std::getline(ifile,str1);
		for(size_t k=0; k<columns; k++){
			bool is_start = (i == y_origin-1 && k == x_origin-1);
			switch(str1[k]){
				case '.':{
					board[i][k] = new Square(1,1,is_start);
					break;
				}
				case 'd':{
					board[i][k] = new Square(1,2,is_start);
					break;
				}
				case 't':{
					board[i][k] = new Square(1,3,is_start);
					break;
				}
				case '2':{
					board[i][k] = new Square(2,1,is_start);
					break;
				}
				case '3':{
					board[i][k] = new Square(3,1,is_start);
					break;
				}
				default:{
					throw std::invalid_argument("error: invalid character detected in board init file");
				}
			}
		}
	}
	ifile.close();
}

std::deque<std::pair<std::string, size_t>> Board::get_place_move_results(PlaceMove &m){
	std::exception e;
	std::deque<Coord> proposal; // tiles and their locations proposed to place them
	std::deque<std::pair<std::string, size_t>> words_and_points;
	// list of words that will be new to the board


	try {
		// these tiles will be placed on coords, retrieve those coords
		proposal = get_proposal(m);
		// make sure that the new word is building off of a current word
		check_connected(m, proposal);
		m.place_tiles(proposal); // place the move tiles on the board

		std::string word_string; // current word being parsed
		size_t word_points = 0; // current word being parsed

		std::deque<std::pair<std::string, size_t>> words_and_points;
		for (int i=0; i<proposal.size(); i++){
			Coord curr = proposal[i];
			word_string=""; // reset the string to be blank
			word_points = 0; // reset the point value to 0

			if (m.direction=='|'){
				while (curr.y!=0 && curr.y!=' '){
					--curr.y; // move to the top-most tile
				} // if user places vertical word
				while(curr.y != rows-1 && curr.y != ' '){
					word_string+=(board[curr.y][curr.x]->getLetter());
					if (curr.x == proposal[i].x and curr.y == proposal[i].y){
						word_points += board[curr.y][curr.x]->getScore() * curr.l_mult;
					} // special case for letter multipliers on new tiles being placed
				} // go from top to bottom, read the letters into the string

				while (!proposal.empty()){
					curr = proposal.front(); // load the proposed tile placement
					proposal.pop_front();
					while (curr.x != 0 && curr.x != ' '){
						--curr.x; // move to the left-most tile
					}
					while (curr.x != columns-1 && curr.x != ' '){
						word_string+=(board[curr.y][curr.x]->getLetter());
						if (curr.x == proposal[i].x and curr.y == proposal[i].y){
							word_points += board[curr.y][curr.x]->getScore() * curr.l_mult;
						} // special case for letter multipliers on new tiles being placed
					} // go from left to right, read the letters into the string
				}
			}

			if (m.direction=='-'){
				while (curr.x != 0 && curr.x != ' '){
					--curr.x; // move to the left-most tile
				} // if user places horizontal word
				while(curr.x != columns-1 && curr.x != ' '){
					word_string+=(board[curr.y][curr.x]->getLetter());
					if (curr.x == proposal[i].x and curr.y == proposal[i].y){
						word_points += board[curr.y][curr.x]->getScore() * curr.l_mult;
					} // special case for letter multipliers on new tiles being placed
				} // go from left to right, read the letters into the string

				while (curr.y != 0 && curr.y != ' '){
					--curr.y; // move to the top-most tile
				}
				while (curr.y != rows-1 && curr.y != ' '){
					if (curr.x == proposal[i].x and curr.y == proposal[i].y){
						word_points += board[curr.y][curr.x]->getScore() * curr.l_mult;
					} // special case for letter multipliers on new tiles being placed
					word_string+=(board[curr.y][curr.x]->getLetter());
					if (curr.x == proposal[i].x and curr.y == proposal[i].y){
						word_points += board[curr.y][curr.x]->getScore() * curr.l_mult;
					} // special case for letter multipliers on new tiles being placed
				}
			}
			word_points *= curr.w_mult;
			words_and_points.push_back(std::make_pair(word_string, word_points));
		}
		check_words_formed(words_and_points);
	}
	catch(&e){ // prompt the user what the error was
		m.redact_tiles(proposal);
		std::cerr << e.what() << std::endl;
		std::deque<std::pair<std::string, size_t>> foo;
		return foo; // nothing to return in this case
	} // return his tiles after removing them from the board
	if (words_and_points.size() == m._player->max_tiles){
		words_and_points.push_back(std::make_pair("*bonus*", 50));
	} // if player used all his tiles in this move, award 50 bonus points
	return words_and_points;
}

// returns the coordinates that the player's tiles will be placed on
std::deque<Coord> Board::get_proposal(PlaceMove& move){
	// check if the input is invalid
	check_stupid(move);

	std::deque<Coord> proposal;
	Coord curr(move.x, move.y, move.str[0]);
	// copy the proposed move's original coordinate
	for (size_t i=0; i<move.str.size(); i++){
		curr.w_mult = 1;
		curr.l_mult = 1;
		while (board[curr.y][curr.x]!=' '){
			if (board[curr.y][curr.x]->getWMult() > curr.w_mult){
				curr.w_mult = board[curr.y][curr.x]->getWMult();
			} // adopt the word multiplier of the board while we're scanning
			if (move.direction == '|'){
				++curr.y;
			} // loop through each letter of the string
			if (move.direction=='-'){
				++curr.x;
			}  // account for already-occupied tiles on the board
			check_out_of_bounds(curr);
		} // avoid seg-faults as we go work our way across the board
		curr.c = move.str[i];
		curr.l_mult = board[curr.y][curr.x]->getLMult();
		proposal.push_back(curr);
	} // add a proposed placement to the list
	return proposal;
}


// attempts to throw exception to some of the more basic illegal moves
void Board::check_stupid(PlaceMove& move){

	if (board[move.y,move.x]!=' '){
		throw std::invalid_argument("error: that tile is already occupied");
	} // check if placing 1st tile on top of another tile
	if (move.str.length()==0){
		throw std::invalid_argument("error: you must play at least one tile");
	} // check if not placing any tiles
	if (!move._player->has_tiles(move.str)){
		throw std::invalid_argument("error: you don't have those tiles");
	} // check if player doesn't have those tiles
	if (move.direction!='-' && move.direction!='|'){
		throw std::invalid_argument("error: invalid direction");
	} // check if player doesn't give a valid direction
	if (move.x >= columns || move.y >= rows){
		throw std::invalid_argument("error: can't place tiles out of range");
	} // check if player tries to place tiles out of range
	if (move.x < 0  || move.y < 0){
		throw std::invalid_argument("error: can't place tiles out of range");
	}
}


// check if the first move is appropriately placed to cover the start tile
bool Board::check_first_move(PlaceMove& move){
	if (board[y_origin][x_origin]->getLetter()!=' '){
		if (move.str.length()<2){
			throw std::invalid_argument("error: you must play at least two tiles");
			return false;
		} // otherwise, this was a valid move to check
		return true;
	} // if it's not the first move, no need to run the rest of the function
	if (move.direction=='|'){
		if (y_origin + move.str.length() < move.y || x_origin != move.x){
			throw std::invalid_argument("error: first move won't touch the start tile");
			return false;
		} // check if any tile would be placed on the origin
	}
	if (move.direction=='-'){
		if (x_origin + move.str.length() < move.x || y_origin != move.y){
			throw std::invalid_argument("error: first move won't touch the start tile");
			return false;
		} // check if any tile would be placed on the origin
	}
	return true;
}


// check if any of the proposed coordinates would land out of bounds
void Board::check_out_of_bounds(Coord& curr){
	if (curr.y >= rows || curr.x >= columns){
		throw std::invalid_argument("error: move would be out of bounds.");
	} 	// check for the potential seg-fault
}



// check if a proposed move will connect to the previous tiles played
bool Board::check_connected(PlaceMove& move, std::deque<Coord> proposal){
	if (check_first_move(move)){
		return true;
	} // no need for the word to connect if it's the first move of the game

	for (size_t i=0; i<proposal.size(); i++){
		if (proposal[i].y != 0){
			if (board[proposal[i].y-1][proposal[i].x]->isOccupied()){
				return true;
			} // if a new tile is connected to a tile on the north side
		}
		if (proposal[i].x != 0){
			if (board[proposal[i].y-1][proposal[i].x]->isOccupied()){
				return true;
			} // if a new tile is connected to a tile on the west side
		}
		if (proposal[i].y != rows - 1){
			if (board[proposal[i].y-1][proposal[i].x]->isOccupied()){
				return true;
			} // if a new tile is connected to a tile on the south side
		}
		if (proposal[i].x != columns - 1){
			if (board[proposal[i].y-1][proposal[i].x]->isOccupied()){
				return true;
			} // if a new tile is connected to a tile on the east side
		}
	} // at this point, we should have returned true if we found a connection
	throw std::invalid_argument("error: move wouldn't connect to the board");
	return false;
}


bool Board::isOccupied(size_t y, size_t x){
	return board[y][x]->isOccupied();
}
