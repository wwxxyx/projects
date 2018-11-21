#include "Move.h"


Move::Move(Player* p){
	_player = p;
}

Move::~Move(){
	delete this;
}


Move* Move::parseMove(std::string moveString, Player &p){
	if (moveString.substr(0,4)=="PASS"){
		return new PassMove(&p);
	}
	else if (moveString.substr(0,8)=="EXCHANGE"){
		return new ExchangeMove(moveString,&p);
	}
	else if (moveString.substr(0,5)=="PLACE"){
		return new PlaceMove(moveString,&p);
	}
	throw std::invalid_argument("error: please enter a valid argument");
}


PassMove::PassMove(Player* p):Move(p){
	_player->set_passed(true);
}


void PassMove::execute(Board& board, Bag& bag, Dictionary& dictionary){

	// board, bag, & dictionary are not used in a pass move
	(void)board;
	(void)bag;
	(void)dictionary;

	return;
}


ExchangeMove::ExchangeMove(std::string tileString, Player* p):Move(p){
	str = tileString.substr(9,tileString.length()-9);
	std::locale loc;
	std::string move_string = move;
	for (size_t i=0; i<move_string.length(); i++){
		move_string[i]=std::tolower(move_string[i],loc);
	}
}


void ExchangeMove::execute(Board & board, Bag & bag, Dictionary & dictionary){
	// board & dictionary are not used in a pass move
	(void)board;
	(void)dictionary;

	if (!_player->hasTiles(str)){
		throw std::invalid_argument("error: you don't have those tiles");
	}
	else{
		// player puts the tiles he wants to exchange into the bag
		bag.addTiles(_player->takeTiles(str));
		// player receives a random set of tiles from the bag
		_player->addTiles(bag.drawTiles(str.length()));
	}
}


PlaceMove::PlaceMove (std::string entry, Player* p): Move(p){
	entry = entry.substr(6,entry.length()-6);
	std::stringstream ss(entry);
	ss >> direction >> y >> x >> str;
	// from here on out, we're using normal CS coordinates
	--x; --y;
}


void PlaceMove::execute(Board& board, Bag& bag, Dictionary& dictionary){
	std::deque<Coord> proposal = get_proposal(this);
	std::deque<std::pair<std::string, size_t>> words_and_points = get_place_move_results(this);
	for(int i=0; i<results.size(); i++){
		std::cerr << "Word placed: " results[i].first << endl;
		std::cerr << "Points added: " results[i].second << endl;
		_player.score += results[i].second
	}
	std::cerr << "New score: " _player.score << std::endl;
}

// place a player's proposed tiles on the board
void PlaceMove::place_tiles(std::deque<Coord> proposal){
	std::vector<Tile*> place_tile = _player->take_tiles(str);
	// places the tiles on the board
	for(size_t i=0; i<proposal.size(); i++){
		board.board[proposal[i].y][proposal[i].x]->placeTile(place_tile[i]);
	}
	_player->add_tiles(bag.drawTiles(proposal.size()));
}

// redact a player's attempted tile placement from the board if an error is detected during the evaluation
void PlaceMove::redact_tiles(std::deque<Coord> proposal){
	std::vector<Tile*> removed_tiles;
	for(size_t i=0; i<proposal.size(); i++){
		removed_tiles.push_back(board.board[proposal[i].y][proposal[i].x]->redactTile(proposal[i]));
	}
	_player->add_tiles(removed_tiles);
}

// looks at new words made, and verifies that they are in the dictionary
void PlaceMove::check_words_formed(std::deque<std::pair<std::string, size_t>> words_and_points){
if (results.size() == 0){
	throw std::invalid_argument("error: no new words are created");
}
for (size_t i=0; i<words_and_points.size(); i++){
	if (!dictionary.is_legal_word(words_and_points[i].first)){
		board.redact_tiles(proposal);
		throw std::invalid_argument("error: a word created was not found in the dictionary");
	}
}
