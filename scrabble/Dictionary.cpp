#include <set>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Dictionary.h"

/* The constructor gets passed the name of the file from which
to read the word list. */
Dictionary::Dictionary (std::string dictionary_filename){
	std::ifstream ifile(dictionary_filename);
	std::string str;
	while (std::getline(ifile,str)){
		words.push_back(str);
	}
	std::sort(words.begin(),words.end());
	ifile.close();
}

Dictionary::~Dictionary(){}

/* Checks whether the given word is in the dictionary, and returns true if so.
Case should not matter. Must run in O(logn) for the number of words in the dictionary. */
bool Dictionary::isLegalWord (std::string const & word) const{
	if (word.length()<2) return false;
	std::string str1 = word;
	str1.erase(std::remove(str1.begin(),str1.end(),' '),str1.end());
	std::transform(str1.begin(),str1.end(),str1.begin(),::tolower);
	return std::binary_search(words.begin(), words.end(), str1);
}
