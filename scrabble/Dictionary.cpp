#include "Dictionary.h"

/* The constructor gets passed the name of the file from which
to read the word list. */
Dictionary::Dictionary(std::string dictionary_filename){
	std::ifstream ifile(dictionary_filename);
	std::string str;
	str.erase(std::remove(str.begin(),str.end(),' '),str.end());
	while (std::getline(ifile,str)){
		bool contains_apostrophe = false;
		for (int i=0; i<str.length(); i++){
			if (str[i]=='\''){
				contains_apostrophe = true;
			} // the Trie.h library can't handle apostrophes in words
		} // unifying the caps/not caps of all words is handled inside Trie.h
		if(!contains_apostrophe){
			words.insert(str);
		} // so don't worry about "APPLE" vs. "apple" the conversion
	}
	ifile.close();
}

Dictionary::~Dictionary(){}

// checks whether the given word is in the dictionary
bool Dictionary::is_legal_word(std::string const & word){
	/* handles edge case: "a" is a real word, but "a" is not a valid word
	to play in a game in scrabble */
	return word.length() >= 2 ? words.prefix(word)!=nullptr : false;
}
