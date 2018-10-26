#include <vector>
#include <string>
#include <map>
#include <deque>
#include <stdio.h>
#include "Board.h"
#include "Bag.h"
#include "Dictionary.h"
#include "Board.h"
#include "Move.h"

class Move;
class Board;

struct Coord{
  Coord();
  size_t x;
  size_t y;
};

class Referee{

public:
  Referee(Board* board, Bag* bag, Dictionary* dictionary);
  ~Referee();
  bool approves(Move* move);
  // check if the ref approves of a proposed move
  size_t resulting_points(Move* move);
  // check how many points a move awards, and allocates them

private:

  // pointers to the elements of the game
  Board* board;
  Bag* bag;
  Dictionary* dictionary;

  std::deque<Coord> get_proposal(Move* move);
  // return coordinate list of tile placements within the current board

  void copy_board();
  // copy's the current state of the actual game board
  void const print_board();
  // prints the current state of the ref's (potentially modified) board
  void check_connected(Move* move);
  // check if the new word will connect to the origin
  void check_words_formed(Move* move);
  // check if the new words formed are in the dictionary
  void check_bounds(Move* move);
  // check if a move would be out of bounds
  void check_stupid(Move* move);
  // check if the move consists of stupid mistakes

  char** ref_board; // copy of the current letters on the game's board
  size_t b_start_x; // the board's x origin (0-indexed)
  size_t b_start_y; // the board's y origin (0-indexed)
  size_t b_height; // the board's height
  size_t b_width; // the board's width
  std::map<char,int> value; // maps point values to letters
};
