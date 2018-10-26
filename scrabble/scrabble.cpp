#include "Bag.h"
#include "Board.h"
#include "ConsolePrinter.h"
#include "Dictionary.h"
#include "Player.h"
#include "Square.h"
#include "Move.h"
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <string>

void announce_winners(std::vector<Player>& player){
  std::vector<int> winners;
  int high_score = player[0].get_score();
  for (size_t i=0; i<player.size(); i++){
    if (player[i].get_score() > high_score){
      high_score = player[i].get_score();
      for (size_t i=0; i<winners.size(); i++){
        winners.pop_back();
      }
      winners.push_back(i);
    }
    else if (player[i].get_score()==high_score){
      winners.push_back(i);
    }
    else continue;
  }
  if (winners.size()==1){
    std::cout << "Winner: " << player[winners[0]].get_name() << std::endl;
    return;
  }
  else if (winners.size()>1){
    std::cout << "Winners: ";
    for (size_t i=0; i<winners.size(); i++){
      std::cout << player[winners[i]].get_name() << " ";
    }
    std::cout << std::endl;
  }
  else throw std::logic_error("error: nobody won");
}


bool everyone_passed(std::vector<Player>& player){
  /* loop through all players, check if they passed */
  for (size_t i=0; i<player.size(); i++){
    if (player[i].get_passed() == false) return false;
  }
  return true;
}

void play(Dictionary& dictionary, Bag& bag, Board& board, std::vector<Player>& player, int handsize){
  std::exception e;
  std::string move_string;
  bool foo = false;

  /* first, allocate a set of tiles to each player */
  for (size_t i=0; i<player.size(); i++){
    player[i].addTiles(bag.drawTiles(size_t(handsize)));
  }
  bool everyone_has_tiles = true;
  bool first_move = true;
  size_t last_player = 0;
  /* this loop continues until the bag runs out of tiles
  and the first player runs out of remaining tiles */
  while(bag.tilesRemaining()>0 && everyone_has_tiles){
    for (size_t i=0; i<player.size(); i++){

      /* new turn, the player hasn't passed yet */
      player[i].set_passed(false);

      ConsolePrinter::printBoard(board);
      ConsolePrinter::printHand(player[i]);
      bool successful_move = false;
      while (!successful_move){
        std::cout << "Please make a move, " << player[i].get_name() << std::endl;
        std::getline(std::cin,move_string);
        try{
          Move* player_turn = player_turn->parseMove(move_string,player[i]);
          player_turn->execute(board,bag,dictionary);
          successful_move = true;
        }
        catch(std::invalid_argument &e){
          std::cout << e.what() << std::endl;
        }
      }
      // make a move object, for this player
      ConsolePrinter::printBoard(board);
      ConsolePrinter::printHand(player[i]);
      std::cout << "Result of your move. Press enter to continue:";
      std::getline(std::cin,move_string);
      move_string = "";
      if (everyone_passed(player)){
        foo = true;
        break;
      }
      /* catches an empty hand, breaks out of the loop */
      if (player[i].getHandTiles().empty()){
        everyone_has_tiles = false;
        last_player = i;
        break;
      }
      else continue;
    }
    /* break out of the game if everyone passed this round */
    if (foo) break;
  }

  /* at this point, the bag has no more tiles and someone used their last one
  alternatively, everyone passe this turn, and the game is over */

  /* next, we will tally the remaining tiles in the players hands,
  remove those tiles from the game, and allocate points accordingly */
  for (size_t i=0; i<player.size(); i++){
    int points = player[i].remaining_hand_total();
    player[last_player].adjust_score(points);
    player[i].adjust_score(points*-1);
  }

  /* announce the winners of the game now that the scores are final */
  std::cout << std::endl;
  announce_winners(player);
}






int main(int argc, char** argv){
  std::exception e;
  if (argc!=2){
    std::cout << "error: invalid # of arguments" << std::endl;
    return -1;
  }
  std::string config_file = std::string(argv[1]);
  std::ifstream ifile(config_file);
  std::string str1;
  std::string bag_filename;
  std::string dictionary_filename;
  std::string board_filename;
  int handsize;
  uint32_t random_seed;

  while(std::getline(ifile,str1)){
    std::string str2 = "";
    if (str1[0]=='H'){
      str1.erase(std::remove(str1.begin(), str1.end(), ' '), str1.end());
      str1.erase(std::remove(str1.begin(), str1.end(), ':'), str1.end());
      for(size_t i=0; i<str1.length(); i++){
        if (isdigit(str1[i])) str2+=str1[i];
      }
      handsize = std::stoi(str2);
    }
    else if (str1[0]=='T'){
      str1.erase(std::remove(str1.begin(), str1.end(), ' '), str1.end());
      str1.erase(std::remove(str1.begin(), str1.end(), ':'), str1.end());
      for(size_t i=5; i<str1.length(); i++){
        str2+=str1[i];
      }
      bag_filename = str2;
    }
    else if (str1[0]=='D'){
      str1.erase(std::remove(str1.begin(), str1.end(), ' '), str1.end());
      str1.erase(std::remove(str1.begin(), str1.end(), ':'), str1.end());
      for(size_t i=10; i<str1.length(); i++){
        str2+=str1[i];
      }
      dictionary_filename = str2;
    }
    else if (str1[0]=='B'){
      str1.erase(std::remove(str1.begin(), str1.end(), ' '), str1.end());
      str1.erase(std::remove(str1.begin(), str1.end(), ':'), str1.end());
      for(size_t i=5; i<str1.length(); i++){
        str2+=str1[i];
      }
      board_filename = str2;
    }
    else if (str1[0]=='S'){
      str1.erase(std::remove(str1.begin(), str1.end(), ' '), str1.end());
      str1.erase(std::remove(str1.begin(), str1.end(), ':'), str1.end());
      for(size_t i=0; i<str1.length(); i++){
        if (isdigit(str1[i])) str2+=str1[i];
      }
      random_seed = std::stoi(str2);
    }
  }
  ifile.close();
  Bag bag(bag_filename, random_seed);
  Dictionary dictionary(dictionary_filename);
  Board board(board_filename);


  int usercount = 0;
  std::vector<Player> player;
  std::string username;
  std::cout << "Please enter the number of players: ";
  std::cin >> usercount;
  if (usercount<1 || usercount>8){
    throw std::invalid_argument("error: invalid # of players");
  }
  std::cout << "Please submit the name of each player, one by one";
  std::cout << std::endl;
  for (size_t i=0; i<usercount; i++){
    std::cin >> username;
    Player* p = new Player(username,handsize);
    player.push_back(*p);
  }
  play(dictionary, bag, board, player, handsize);
  while (player.size()>0){
    player.pop_back();
  }
  return 0;
}
