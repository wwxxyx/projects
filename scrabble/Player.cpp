#include "Player.h"

Player::Player (std::string name, size_t maxTiles){
this->name = name;
this->maxTiles = maxTiles;
}


Player::~Player(){}


// returns set of tiles the player currently holds
std::set<Tile*> Player::getHandTiles() const{
std::set<Tile*> tiles;
for(size_t i=0; i<hand.size(); i++){
tiles.insert(hand[i]);
}
return tiles;
}


// checks if a player has the tiles in "move"
bool Player::hasTiles(std::string & move){
std::locale loc;
std::string move_string = move;
std::string hand_string = "";
for (size_t i=0; i<move_string.length(); i++){
if (move_string[i]=='?'){
throw std::invalid_argument("error: this program does not (yet) support '?' as a letter");
}
move_string[i]=std::tolower(move_string[i],loc);
}
for(size_t i=0; i<hand.size(); i++){
hand_string+=hand[i]->getLetter();
}
while (move_string.size()>0){
char foo = move_string.back();
move_string.pop_back();
bool found_it = false;
for (size_t i=0; i<hand_string.size(); i++){
if (hand_string[i]==foo){
hand_string.erase(i,1);
found_it = true;
break;
}
}
if (found_it==false) return false;
}
return true;
}


// takes away the tiles in "move" from the player
std::vector<Tile*> Player::takeTiles (std::string const & move){
std::locale loc;
std::string move_string = move;
for (size_t i=0; i<move_string.length(); i++){
move_string[i]=std::tolower(move_string[i],loc);
}
std::vector<Tile*> returnTiles;
for (size_t i=0; i<move_string.size(); i++){
for (size_t k=0; k<hand.size(); k++){
if (hand[k]->getLetter()==move_string[i]){
returnTiles.push_back(hand[k]);
hand.erase(hand.begin()+k);
break;
// remove it from the player's hand
}
}
}
return returnTiles;
}


// adds all the tiles in the vector to the player's hand.
void Player::addTiles (std::vector<Tile*> const & tilesToAdd){
for (size_t i=0; i<tilesToAdd.size(); i++){
hand.push_back(tilesToAdd[i]);
}
}



int Player::remaining_hand_total(){
int points = 0;
while (hand.size()>0){
points+=hand[0]->getPoints();
delete hand[0];
hand.erase(hand.begin());
}
return points;
}
