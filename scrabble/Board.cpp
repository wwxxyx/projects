#include "Board.h"


/* constructor that initializes the board state with an empty board
and the size and bonuses and square based on the given file. */
Board::Board(std::string filename){
	std::ifstream ifile(filename);
	ifile >> columns;
	ifile >> rows;
	ifile >> start_x;
	ifile >> start_y;
	std::string str1;
	std::getline(ifile,str1);
	square.resize(rows,std::vector<Square*>(columns,0));
	for(size_t i=0; i<rows; i++){
		std::getline(ifile,str1);
		for(size_t k=0; k<columns; k++){
			bool is_start = (i == start_y-1 && k == start_x-1);
			if(str1[k]=='d') square[i][k]=new Square(1,2,is_start);
			else if(str1[k]=='t') square[i][k]=new Square(1,3,is_start);
			else if(str1[k]=='2') square[i][k]=new Square(2,1,is_start);
			else if(str1[k]=='3') square[i][k]=new Square(3,1,is_start);
			else square[i][k]=new Square(1,1,is_start);
		}
	}
	for (int a=0; a<getRows(); a++){
		char_board.push_back("");
		{
		for (int b=0; b<getColumns(); b++){
			char_board[a]+=getSquareChar(a,b);
	}
}
	ifile.close();
}
}

Board::~Board (){
	for(size_t i=0; i<rows; i++){
		for(size_t k=0; k<columns; k++){
			delete square[i][k];
		}
	}
}


/* Returns a pointer to the Square object representing the
(y,x) position of the board. Indexing is_starts at 1 here.
This is needed only to display the board. */
Square* Board::getSquare (size_t y, size_t x) const{
	return square[y-1][x-1];
}

char Board::getSquareChar(size_t y, size_t x){
	if (square[y][x]->getLetter() == ' ') return '#';
	else return getSquare(y,x)->getLetter();
}

/* Returns the number of rows of the board.*/
size_t Board::getRows() const{
	return rows;
}

/* Returns the number of columns of the board.*/
size_t Board::getColumns() const{
	return columns;
}

bool Board::isOccupied(size_t y, size_t x){
	return getSquare(y,x)->getLetter()!=' ';
}

void Board::print_chars(){
	for (size_t i=0; i<char_board.size(); i++){
		std::cout << char_board[i] << std::endl;
	}
}

/* returns a vector of all words that would be formed by executing the
given move. The first element of the pair is a string containing the word
formed, and the second element is the score that that word earns
(with all multipliers, but not the 50-point bonus for all letters).

Words returned are all in uppercase.

The last entry of the vector is always the "main" word formed
in the direction chosen by the user; the others could be in arbitrary
order. (This is helpful for backtracking search.)

This function does not check that the words formed are actually in the dictionary.
The words returned from this function must be checked against the dictionary to
determine if the move is legal.	*/
