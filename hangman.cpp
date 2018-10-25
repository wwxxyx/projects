#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

int processGuess(char* word, const char* targetWord, char guess);
bool guessCheck(char guess,char* word);

const char* wordBank[] = {"charlie", "itslitfam", "imdead", "surf",
"fronting", "adderall", "guitar", "fuccboi",
"whatsthatafrat", "perf"};
const int numWords = 10;

int main()
{
  srand(time(0));
  char guess;
  int turnCount = 10;

  const char* targetWord = wordBank[rand() % numWords];
  // choose random word from the wordBank
  char word[80];
  // blank array used to build up the answer
  strcpy(word,targetWord);
  for (int i=0;i<strlen(word);i++){
    *(word+i)='*';
  }
  while (true){

    // prompt the user
    cout << "Current word: " << word << endl;
    cout << turnCount << " turns remain... Enter a letter to guess: " << endl;
    cin >> guess;

    // checks if the user is a bitch and entered the same character
    if(guessCheck(guess,word)){
      cout << "bitch you guessed it" << endl;
      turnCount--;
      continue;
    }
    if (processGuess(word,targetWord,guess)==0){
      turnCount--;
    }
    if (strcmp(word,targetWord)==0){
      cout << "The word was: " << targetWord << ". You win!" << endl;
      return 0;
    }
    else if (turnCount==0){
      cout << "Too many turns...You lose!" << endl;
      return 0;
    }
    else{
      continue;
    }
  }
return 0;
}

// Given the current version of the word, target/selected word, and
// the character that the user just guessed, change the word to
// "turn on" any occurrences the guess character and return a count
// of how many occurrences of 'guess' were found
int processGuess(char* word, const char* targetWord, char guess)
{
  int rightCount=0;
  for (int i=0; i<strlen(targetWord); i++){
    if (*(targetWord+i)==guess){
      *(word+i)=guess;
      rightCount++;
    }
  }
  return rightCount;
}

bool guessCheck(char guess,char* word){
  for (int i=0; i<strlen(word); i++){
    if (guess==*(word+i)){
      return true;
    }
  }
  return false;
}
