#include "Move.h"


Move::Move(Player* p){
	_player = p;
}

Move::~Move(){
	delete this;
}


Move* Move::parseMove(std::string moveString, Player &p){
	if (moveString.substr(0,4)=="PASS") return new PassMove(&p);
	else if (moveString.substr(0,8)=="EXCHANGE") return new ExchangeMove(moveString,&p);
	else if (moveString.substr(0,5)=="PLACE") return new PlaceMove(moveString,&p);
	else throw std::invalid_argument("error: please enter a valid argument");
}


PassMove::PassMove(Player* p):Move(p){
	_player->set_passed(true);
}


void PassMove::execute(Board& board, Bag& bag, Dictionary& dictionary){
	return;
}


ExchangeMove::ExchangeMove(std::string tileString, Player* p):Move(p){
	str = tileString.substr(9,tileString.length()-9);
}


void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	if (!_player->hasTiles(str)){
		throw std::invalid_argument("error: you don't have those tiles");
	}
	else{
		bag.addTiles(_player->takeTiles(str));
		_player->addTiles(bag.drawTiles(str.length()));
	}
}


PlaceMove::PlaceMove (std::string entry, Player* p):Move(p){
	entry = entry.substr(6,entry.length()-6);
	std::stringstream ss(entry);
	ss >> direction >> y >> x >> str;
}


void PlaceMove::execute(Board& board, Bag& bag, Dictionary& dictionary){
	check_legality(board, dictionary);
	check_connectivity(board, dictionary);
	std::vector<Tile*> place_tile = _player->takeTiles(str);

	size_t displacement = 0;

	// first, check if it's vertical or horizontal
	// if it's vertical, go down from the x, y
	// if a tile is taken, skip that tile

	size_t total_points = 0;
	size_t W_mult = 1;
	size_t word_points = 0;

	// places the tiles on the board
	for(size_t i=0; i<place_tile.size(); i++){
		if (direction=='|'){
			if (y+i+displacement>board.getRows()){
				throw std::logic_error("error: out of bounds");
			}
			else if (board.getSquare(y+i+displacement,x)->isOccupied()){
				displacement++;
				continue;
			}
			else{
				if (board.getSquare(y+i+displacement,x)->getWMult() > W_mult){
					W_mult = board.getSquare(y+i+displacement,x)->getWMult() > W_mult;
				}
				board.getSquare(y+i+displacement,x)->placeTile(place_tile[i]);
				word_points+=(board.getSquare(y+i+displacement,x)->getScore() * board.getSquare(y+i+displacement,x)->getLMult());
			}
		}
		if (direction=='-'){
			if (x+i+displacement>board.getColumns()){
				throw std::logic_error("error: out of bounds");
			}
			else if (board.getSquare(y,x+i+displacement)->isOccupied()){
				displacement++;
				continue;
			}
			else{
				if (board.getSquare(y,x+i+displacement)->getWMult() > W_mult){
					W_mult = board.getSquare(y,x+i+displacement)->getWMult() > W_mult;
				}
				board.getSquare(y,x+i+displacement)->placeTile(place_tile[i]);
				word_points+=(board.getSquare(y,x+i+displacement)->getScore() * board.getSquare(y, x+i+displacement)->getLMult());
			}
		}
		_player->adjust_score(word_points*W_mult);
	}
	// print();
	_player->addTiles(bag.drawTiles(str.length()));
	// take the tiles from the player
	// place the tiles we took from the player on the board
	// resolve ? wildcards (way back before this function)

}




// lists the words on the char_board (works!)
std::vector<PlaceMove::Word_Coord> PlaceMove::list_all_words(Board& board){
	std::vector<Word_Coord> word_list;
	for (int y=0; y<board.getRows(); y++){
		std::string new_word;
		for (int x=0; x<board.getColumns(); x++){
			if (board.char_board[y][x]!='#'){
				new_word+= board.char_board[y][x];
				if (x==board.getColumns()-1 && new_word.length()>1){
					Word_Coord new_word_coord;
					new_word_coord.word = new_word;
					new_word_coord.x_coord = x;
					new_word_coord.y_coord = y;
					new_word_coord.direction = '-';
					word_list.push_back(new_word_coord);
					new_word = "";
				}
			}
			else if (new_word.length()>1){
				Word_Coord new_word_coord;
				new_word_coord.word = new_word;
				new_word_coord.x_coord = x;
				new_word_coord.y_coord = y;
				new_word_coord.direction = '-';
				word_list.push_back(new_word_coord);
				new_word = "";
			}
			else new_word="";
		}
	}
	for (int x=0; x<board.getColumns(); x++){
		std::string new_word;
		for (int y=0; y<board.getRows(); y++){
			if (board.char_board[y][x]!='#'){
				new_word+= board.char_board[y][x];
				if (y==board.getColumns()-1 && new_word.length()>1){
					Word_Coord new_word_coord;
					new_word_coord.word = new_word;
					new_word_coord.x_coord = x;
					new_word_coord.y_coord = y;
					new_word_coord.direction = '-';
					word_list.push_back(new_word_coord);
					new_word = "";
				}
			}
			else if (new_word.length()>1){
				Word_Coord new_word_coord;
				new_word_coord.word = new_word;
				new_word_coord.x_coord = x;
				new_word_coord.y_coord = y;
				new_word_coord.direction = '|';
				word_list.push_back(new_word_coord);
				new_word = "";
			}
			else new_word="";
		}
	}
	return word_list;
}

// check if the words attempted to be placed are legal (testing!)
void PlaceMove::check_legality(Board& board, Dictionary& dictionary){
	if (board.getSquare(y,x)->isOccupied()){
		throw std::invalid_argument("error: that tile is already occupied");
	}
	if (str.length()==0){
		throw std::invalid_argument("error: you must play at least one letter");
	}
	if (!_player->hasTiles(str)){
		throw std::invalid_argument("error: you don't have those tiles");
	}
	if (direction!='-' && direction!='|'){
		throw std::invalid_argument("error: invalid direction");
	}
	std::vector<Word_Coord> old_words = list_all_words(board);
	// at this point, all words currently on the board are saved in word_list

	for (size_t i=0; i<str.size(); i++){
		size_t ycoord = y-1;
		size_t xcoord = x-1;
		if (direction=='|'){
			if (!board.isOccupied(board.start_y,board.start_x) && (xcoord!=(board.start_x-1))){
				if(i==str.size()-1 && ycoord < board.start_y-1){
					throw std::invalid_argument("error: first move won't touch the start tile");
				}
			}
			while (board.char_board[ycoord][xcoord]!='#'){
				ycoord+=1;
				if (ycoord > board.getRows()-1){
					throw std::invalid_argument("error: move would be out of bounds.");
				}
			}
			board.char_board[ycoord][xcoord]=str[i];
		}
		if (direction=='-'){
			if (!board.isOccupied(board.start_y,board.start_x) && (ycoord!=(board.start_y-1))){
				if(i==str.size()-1 && xcoord < board.start_x-1){
					throw std::invalid_argument("error: first move won't touch the start tile");
				}
			}
			while (board.char_board[ycoord][xcoord]!='#'){

				xcoord+=1;
				if (xcoord > board.getColumns()-1){
					throw std::invalid_argument("error: move would be out of bounds.");
				}
			}
			board.char_board[ycoord][xcoord]=str[i];
		}
	}
	// at this point, the char_board is updated with the new inputs

	std::vector<Word_Coord> new_words = list_all_words(board);
	for(int i=0; i<new_words.size(); i++){
		if (!dictionary.isLegalWord(new_words[i].word)){
			throw std::invalid_argument("error: one or more words created not legal");
		}
	}

	// remove all words that were already on the board
	for(int i=0; i<new_words.size(); i++){
		if (old_words.size()==0) break;
		for (int k=0; k<old_words.size(); k++){
			if ((new_words[i].word == old_words[k].word) && \
			(new_words[i].x_coord == old_words[k].x_coord)){
				if ((new_words[i].y_coord == old_words[k].y_coord) && (new_words[i].direction == old_words[k].direction)){
					new_words.erase(new_words.begin()+i);
					i-=1;
				}
			}
		}
	}
	if (new_words.size()==0) throw std::logic_error("error: no new words created");
	// make sure that a new word was actually created
}

void PlaceMove::check_connectivity(Board& board,Dictionary& dictionary){
	return;
}
