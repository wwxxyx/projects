#include "Player.h"

Player::Player (std::string name, size_t maxTiles){
  this->name = name;
  this->maxTiles = maxTiles;
  this->passed = false;
  this->score = 0;
}
Player::~Player(){
  while (!hand.empty()){
    hand.pop_back();
  }
}

/* return the remaining points left in a player's hand at end of game
and deletes those tiles from the player's hand */
int Player::remaining_hand_total_points(){
  int points = 0;
  while (hand.size()>0){
    points+=hand[0]->getPoints();
    delete hand[0];
    hand.erase(hand.begin());
  }
  return points;
}

// return the set of tiles that a player currently holds
std::set<Tile*> Player::get_hand_tiles(){
  std::set<Tile*> tiles;
  for(size_t i=0; i<hand.size(); i++){
    tiles.insert(hand[i]);
  }
  return tiles;
}

// check if a player has the tiles in "move"
bool Player::has_tiles(std::string & move){
  for (size_t i=0; i<move_string.length(); i++){
    if (move_string[i]=='?'){
      throw std::invalid_argument("error: this program does not (yet) support '?' as a letter");
    }
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

// take away the tiles in "move" from the player
std::vector<Tile*> Player::take_tiles(std::string & move){
  std::string move_string = move;
  if(!has_tiles(move_string)){
    throw std::exception("error: player doesn't have those tiles");
  }
  else {
    std::vector<Tile*> return_tiles;
    for (size_t i=0; i<move_string.size(); i++){
      for (size_t k=0; k<hand.size(); k++){
        if (hand[k]->getLetter()==move_string[i]){
          return_tiles.push_back(hand[k]);
          hand.erase(hand.begin()+k);
          break;
        } // remove the tile from the player's hand
      } // move on to the next tile to search for
    }
  }
  return return_tiles;
}

// add all the tiles in the vector to the player's hand.
void Player::add_tiles(std::vector<Tile*> & tiles_to_add){
  if(tiles_to_add.size() + hand.size() > max_tiles){
    throw std::exception("error: too many tiles added to player's hand");
  }
  else {
    for (size_t i=0; i<tiles_to_add.size(); i++){
      hand.push_back(tiles_to_add[i]);
    }
  }
}
