/*
* Created on: Nov 19, 2018
* Author: Austin Traver
* Dictionary encodes the list of all legal words
*/

#ifndef DICTIONARY_H_
#define DICTIONARY_H_

#include "Trie.h"
#include <fstream>


struct Dictionary {
	/* The constructor gets passed the name of the file from which
	to read the word list. */
	Dictionary (std::string dictionary_filename);
	~Dictionary ();
	/* Checks whether the given word is in the dictionary, and returns true if so.
	Case should not matter. Must run in O(logn) for the number of words in the dictionary. */
	bool is_legal_word(std::string const & word);
	/* The actual set of words, represented as a trie data structure */
	TrieSet words;
};
#endif /* DICTIONARY_H_ */
