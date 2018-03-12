#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>


using namespace std;

class Game{
public:
	Game(); // default constructor

	Game(int level, int lifeline);

	//returns true if word is added to vector
	//needs to check for valid word; valid word = length >= 3 and not duplicate, all lower case, no numbers, no punctuation
	void addWord(string word);

	//returns true if answer is correct
	bool checkAnswer(string answer);

	//spits out the contents of the vector
	void readWords();

	//returns true if game is over
	bool isOver();

	//return the next scrambled word.
	string newWord(int);

	//starts/continues the game.
	void play();

	//generates a vector of with levels defined by 3 words of the same length
	void buildLevels();
	bool isDuplicate(string);
	void printStats();
	void promptGuess(string);
	void useLifeline();






private:
	int level;
	vector<string> Words;
	int currentIndex;
	int lifeline;
	bool gameStatus;
	int guesses;
	int numCorrect;
	string currentWord;
	vector< vector<string> > levelWords;
	int max_len;
	int max_level;
	int max_completed_len;
	int totalCorrect;
	bool lifelineStatus;
	vector<string> copyLevelWords;







};

#endif