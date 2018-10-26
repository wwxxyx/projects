#include "Referee.h"

// TODO
// build value, a map, constructed by parsing bag for each element in it
// build resulting_points(), a method, parsing a move and returning the points


Referee::Referee(Board* board, Bag* bag, Dictionary* dictionary){
  this->board = board;
  this->bag = bag;
  this->dictionary = dictionary;
  this->b_height = board->getRows();
  this->b_width = board->getColumns();
  this->b_start_y = board->start_y-1; // store the start's y coord
  this->b_start_x = board->start_x-1; // store the start's x coord
  char** ref_board = new char*[height];
  for (size_t y=0; y<height; y++){
    ref_board[y] = new char[width];
  }
}


Referee::~Referee(){
  for (size_t; y=0; y<b_height; y++){
    delete[] game_board[y];
  }
  delete[] game_board;
}


// returns true if the move is deemed legal by the referee
bool Referee::approves(Move* move){
  std::exception e;
  try{
    check_invalid(move);
    check_first_move(move);
    check_connected(move);
    check_boundaries(move);
    check_words_formed(move);
  }
  catch(&e){
    std::cout << e.what() << std::endl;
    return false;
  }
  return true;
}

// check how many points a move awards, and allocates them
size_t resulting_points(Move* move){}

// copy the board in its current state
// WARNING will wipe the attempted move's word placement
void Referee::copy_board(){
  for (size_t y=0; y<b_height; y++){
    for (size_t x=0; x<b_width; x++){
      ref_board[y][x] = game_board->getSquare(y+1,x+1)->getLetter();
    }
  }
}


// print the referee's board in its current state
// WARNING may not be identical to board at that moment
void Referee::print_board(){
  for (size_t y=0; y<b_height; y++){
    for (size_t x=0; x<b_height; x++){
      printf(ref_board[y][x]);
    }
    printf('\n');
  }
}



// return coordinate list of tile placements within the current board
// WARNING must call checks before this, or it could seg-fault
std::deque<Coord> Referee::get_proposal(Move* move){
  copy_board(); // reset the ref's board to match the real board
  std::list<Coord> proposals;
  Coord curr; // current coordinate of attempted place
  curr.y = move->get_y()-1;
  curr.x = move->get_x()-1;
  // copy the proposed move's origin
  proposals.push_back(curr); // add it to the list of coords proposed

  // loop through each letter of the string
  for (size_t i=0; i<move->get_str().size(); i++){
    while (ref_board[curr.y][curr.x]!=' '){
      if (move->get_dir() == '|') ++curr.y;
      if (move->get_dir()=='-') ++curr.x;
    } // loop through the placement, account for already-occupied tiles
    proposals.push_back(curr); // add a proposed placement to the list
    board.char_board[curr.y][curr.x]='.'; // mark a proposed placement
  }
  return proposals;
}


// returns the new words formed on the board after a proposed move is executed
std::deque<std::string> Referee::new_words_formed(Move* move){
  copy_board(); // copy the board before placing the tiles
  place_tiles(move); // place the move tiles on the board
  std::string curr_word; // current word being parsed

  // list of coordinates that tiles would like to be placed
  std::deque<Coord> proposal = get_proposal(move);
  std::deque<std::string> word_list; // list of words will be new to the board
  Coord curr = proposal.front();


  if (move->get_dir()=='|'){
    while (curr.y!=0 and curr.y!=' '){
      --curr.y; // move to the top-most tile
    }
    while(curr.y!=height-1 and curr.y!=' '){
      curr_word.append(ref_board[curr.y][curr.x])
    } // go from top to bottom, read the letters into the string
    word_list.push_back(curr_word); // add the string to the words formed
    curr_word=""; // reset the string to be blank

    while (not proposal.empty()){
      curr = proposal.front(); // load the proposed tile placement
      proposal.pop_front();
      while (curr.x!=0 and curr.x!=' '){
        --curr.x; // move to the left-most tile
      }
      while (curr.x!=width-1 and curr.x!=' '){
        curr_word.append(ref_board[curr.y][curr.x])
      } // go from left to right, read the letters into the string
      word_list.push_back(curr_word); // add the string to the words formed
      curr_word=""; // reset the string to be blank
    }

  }
  if (move->get_dir()=='-'){
    while (curr.x!=0 and curr.x!=' '){
      --curr.x; // move to the left-most tile
    }
    while(curr.x!=width-1 and curr.x!=' '){
      curr_word.append(ref_board[curr.y][curr.x])
    } // go from left to right, read the letters into the string
    word_list.push_back(curr_word); // add the string to the words formed
    curr_word=""; // reset the string to be blank

    while (not proposal.empty()){
      curr = proposal.front(); // load the proposed tile placement
      proposal.pop_front();
      while (curr.y!=0 and curr.y!=' '){
        --curr.y; // move to the top-most tile
      }
      while (curr.y!=height-1 and curr.y!=' '){
        curr_word.append(ref_board[curr.y][curr.x])
      } // go from top to bottom, read the letters into the string
      word_list.push_back(curr_word); // add the string to the words formed
      curr_word=""; // reset the string to be blank
    }
  }
}


// attempts to throw exception to some of the more basic illegal moves
void Referee::check_stupid(Move* move){
  copy_board();
  if (ref_board[move->get_y()-1,move->get_x()-1]!=' '){
    throw std::invalid_argument("error: that tile is already occupied");
  } // check if placing 1st tile on top of another tile
  if (move->get_str().length()==0){
    throw std::invalid_argument("error: you must play at least one tile");
  } // check if not placing any tiles
  if (not move->get_player()->hasTiles(move->get_str())){
    throw std::invalid_argument("error: you don't have those tiles");
  } // check if player doesn't have those tiles
  if (move->get_dir()!='-' and move->get_dir()!='|'){
    throw std::invalid_argument("error: invalid direction");
  } // check if player doesn't give a valid direction
  if (move->get_x()>width || move->get_y()>height){
    throw std::invalid_argument("error: can't place tiles out of range")
  }
  if (move->get_x()<0 || move->get_y()<0){
    throw std::invalid_argument("error: can't place tiles out of range")
  }
}


// check if the first move is appropriately placed to cover the start tile
void Referee::check_first_move(Move* move){
  copy_board();
  Coord start; // current coordinate of attempted place
  start.y = move->get_y()-1;
  start.x = move->get_x()-1;

  // check if the board's start tile is blank
  if (ref_board[b_start_y][b_start_x]==' '){
    if (move->get_dir()=='|'){
      if (start.y + move->get_str().length() < b_start_y or start.x!=b_start_x)){
        throw std::invalid_argument("error: first move won't touch the start tile");
      }
    }
    if (move->get_dir()=='-'){
      if (start.x + move->get_str().length() < b_start_x or start.y!=b_start_y){
        throw std::invalid_argument("error: first move won't touch the start tile");
      }
    }
  }
}


// check if any of the proposed coordinates would land out of bounds
void Referee::check_boundaries(Move* move){
  copy_board(); // reset the board to match the real board

  Coord curr; // current coordinate attempting to be placed;
  curr.y = move->get_y()-1;
  curr.x = move->get_x()-1;

  // loop through each letter of the string
  for (size_t i=0; i<move->get_str().size(); i++){

    // while already-filled tiles are present
    while (ref_board[curr.y][curr.x]!=' '){
      // increment the coordinate accordingly
      if (move->get_dir()=='|') ++curr.y;
      if (move->get_dir()=='-') ++curr.x;
      // check for the potential seg-fault
      if (curr.y > height or curr.x > width){
        throw std::invalid_argument("error: move would be out of bounds.");
      }
    }
    board.char_board[curr.y][curr.x]='.';
  } // otherwise mark the proposed placement
}


// looks at new words made, and verifies that they are in the dictionary
void Referee::check_words_formed(Move* move){
  std::deque<std::string> word_list = new_words_formed(move);
  while (not word_list.empty()){
    if (!dictionary->isLegalWord(wordList.front())){
      throw std::invalid_argument("error: one or more words created not legal");
    }
    else wordList.pop_front();
  }
}


// check if a proposed move will connect to the previous tiles played
void Referee::check_connected(Move* move){
  copy_board();
  place_tiles(move); // WARNING doesn't exist yet!

  // TODO this won't work yet. You need have this place the proposed move
  // on the ref_board and then continue check_connected

  // initialize a deck of coordinates
  std::deque<Coord> q;
  Coord start, end, curr, next;
  start.x = start_x;
  start.y = start_y;
  end.x = board_x;
  end.y = board_y;

  if (start.y == end.y && start.x == end.x) return;
  // edge case, tile placed is actually the origin

  // we know start is not origin, but it gets the ball rolling
  q.push_back(start);

  // loop until we've run out of potential paths to origin
  while (not q.empty()){
    curr = queue.front();
    queue.pop_front();
    if (ref_board[curr.y][curr.x]==' ') continue; // check if a blank space
    else if (ref_board[curr.y][curr.x]=='0') continue; // prev. visited...
    else if (curr.y == end.y && curr.x == end.x){
      // path found!
      return;
    }
    else{
      ref_board[curr.y][curr.x]='0';
      // mark this as a visited tile

      /* next, check the tiles around the current tile, the booleans serve to
      avoid potential seg-faults around the edge of the board */
      if (curr.y!=0){
        next.y = curr.y-1;
        next.x = curr.x;
        q.push_back(next);
      }
      if (curr.y!=height-1){
        next.y = curr.y+1
        next.x = curr.x;
        q.push_back(next);
      }
      if (curr.x!=0){
        next.y = curr.y;
        next.x = curr.x-1
        q.push_back(next);
      }
      if (curr.x!=width-1){
        next.y = curr.y;
        next.x = curr.x+1;
        q.push_back(next);
      }
    }
  } // triggered if we ran out of tiles to find a path with
  throw std::exception("error: move won't be connected");
}
