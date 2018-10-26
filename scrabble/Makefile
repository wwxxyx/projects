all: scrabble

scrabble: scrabble.cpp Bag.o Board.o ConsolePrinter.o Dictionary.o Player.o Move.o
	g++ -g -Wall -w -std=c++11 $^ -o $@
	rm -rf *.o *.dSYM

%.o: %.cpp
	g++ -c -g -Wall -w -std=c++11 $< -o $@
