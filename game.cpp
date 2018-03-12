#include "game.h"

//initializes the game
Game::Game(){
	guesses = 3;
	level = 0;
	lifeline = 5;
	gameStatus = false;
	numCorrect = 0;
	currentWord = "";
	max_len = 0;
	max_level = 0;
	max_completed_len = 0;
	lifelineStatus = false;
	//cout << "constructing game object with: level= "<< level << " lifeline: " << lifeline << endl;
}

Game::Game(int lvl, int life){
	guesses = 3;
	level = lvl;
	lifeline = life;
	gameStatus = false;
	numCorrect = 0;
	currentWord = "";
	max_len = 0;
	max_level = 0;
	max_completed_len = 0;
	lifelineStatus = false;
	//load in the words for a saved game.
	ifstream oldData;
	oldData.open("game.data");
	if(oldData.fail()){
		cerr << "The game data file could not be found or opened.\n";
		exit(1);
	}
	while(!oldData.eof()){
			string word;
			getline(oldData,word);
			addWord(word);
	}

	//cout << "constructing game object with: level= "<< level << " lifeline: " << lifeline << endl;
}
bool Game::isDuplicate(string word){
	for(int i = 0;i< Words.size();i++){
		if(Words[i] == word)
			return true;
	}
	return false;
}

//adds valid words to the game
void Game::addWord(string word){
	string moddedWord;
	for(int i = 0;i< word.length();i++){
		if(word[i] >= 97 && word[i] <= 122){
			moddedWord += word[i];
		}
	}
	if(moddedWord.length() > max_len)
			max_len = moddedWord.length();
	

	if(moddedWord.length() > 3 && !isDuplicate(moddedWord)){

		Words.push_back(moddedWord);
	}

	
}
void Game::buildLevels(){
	for(int i = 4; i< max_len;i++){
		vector<string> aLevel;
		//cout << "beginning of words of length: " << i << endl;
		for(int j = 0; j<Words.size();j++){
			if(Words[j].length()-1 == i){
				//cout << j << " : " << Words[j] << endl;
				//if(aLevel.size() == 8){
				//	break;
				//}
				aLevel.push_back(Words[j]);

			}
		}

		if(aLevel.size() < 3){
			break;
		}
		//read out the vector for the current level
		//for(int k = 0; k < aLevel.size() ; k++){
		//	cout << k << " : " << aLevel[k] << endl;
		//}
		const vector<string> oneLevel = aLevel;
		levelWords.push_back(oneLevel);
		//cout << "ending of words of length: " << i << endl;

	}
	//set max level after finding all the possible difficulties
	max_level = levelWords.size()-1;
	if(level < 0 || level > max_level){
		cerr << "invalid level from config\n";
		gameStatus = true;
	}
}

void Game::readWords(){
	for(int i = 0;i<levelWords.size();i++){
		cout << "level: " << i << endl;
		for(int j = 0;j<levelWords[i].size();j++){
			cout <<  j  << " : " << levelWords[i][j] << endl;
		}
	}
	//cout << "level: " << level << endl;
	//cout << "guesses: " << guesses << endl;
	//cout << "lifeline: " << lifeline << endl;
	//cout << "max_len: " << max_len << endl;
}

bool Game::isOver(){
	return gameStatus;
}

bool Game::checkAnswer(string answer){
	if(answer == currentWord){
		return true;
	}
	return false;
}


string Game::newWord(int level){
	srand(time(NULL));
	vector<string> *list = &levelWords[level];
	//cout << "generating index for level: " << level << " level vector size: " <<levelWords[level].size()<< endl;
	int rand_index = rand() % (list->size());
	//cout << "rand_index: "<<rand_index << " : " << "actual level: "<<level <<" : "<<  "words remaining @ lvl: "<<levelWords[level].size() << endl;
	currentWord = (*list)[rand_index];
	list->erase(list->begin()+rand_index);
	copyLevelWords.push_back(currentWord);
	
	//list->erase(list->begin()+rand_index);
	cout << rand_index << " : " << currentWord << endl;
	string scrambled = currentWord;
	random_shuffle(scrambled.begin(),scrambled.end());
	list = NULL;
	delete list;

	return scrambled;


}
void Game::printStats(){
	//cout << "--stats begin---" << endl;
	//cout << "level: " << level << endl;
	//cout << "guesses: " << guesses << endl;
	//cout << "numCorrect: " << numCorrect << endl;
	//cout << "Lifeline(s) remaining: " << lifeline << endl;
	//cout << "Longest length completed: " << max_completed_len << endl;
	//cout << "total number completed: " << totalCorrect << endl;
	//cout << "--stats end-----" << endl;
	int lvl = level + 1;
	int max_lvl = max_level +1;
	cout << "===| Level: " << lvl <<"/" << max_lvl
		 << " | Solved: " << numCorrect 
		 <<" | Guesses: " << guesses 
		 << " | Lifelines: " << lifeline << " |===\n"
		 << "===| Total Solved: " << totalCorrect
		 << " | Lengthiest Solved: " << max_completed_len
		 <<  " |===\n";
		// cout <<"copied vector contents" << endl;
	//for(int i = 0; i < copyLevelWords.size();i++){
	//	cout <<i << " : "<< copyLevelWords[i] << endl;
	//}
	//cout <<"copied vector contents end" << endl;
	//for(int j = 0;j<levelWords[level].size();j++){
	//	cout << j <<" : " << levelWords[level][j] << endl;
	//}

}
void Game::useLifeline(){
	if(levelWords[level].empty() && !copyLevelWords.empty()){
		srand(time(NULL));
		int rand_index = rand() % copyLevelWords.size();
		string lifeword = copyLevelWords[rand_index];
		promptGuess(lifeword);
	}


}
void Game::promptGuess(string uniqueWord){
	//cout << "===| Level: " << level << " | Guesses: " << guesses << " | Lifelines: " << lifeline << " |===\n"; 
	cout << "challenge: " << uniqueWord << endl << "response: ";
	//read input and determine if it is a command.
		string guess;
		cin >> guess; 
		//check answer or a possible command to exit
		if(guess.length() <= 2){
			if(guess == "x"){
				cout << "signal to exit! " << endl;
				gameStatus = true;
			}else if(guess == "ll"){
				//cout << "use lifeline " << endl;
				lifeline -= 1;
				if(lifeline < 0 && guesses > 0){
					gameStatus = true;
					cout << "out of lifelines; try again!" << endl;
				}else{
					//use lifeline
					useLifeline();
				}
			}
		}else if(!checkAnswer(guess)){
			//need logic to stay on level if wrong up to three times
			// need leveling up logic
			// need to handle bad config inputs
			guesses -= 1;
			if(guesses <= 0){
				gameStatus = true;
				return;
			}
				printStats();
				promptGuess(uniqueWord);
		
		}else{
			cout << "response is correct!" <<endl;
			guesses = 3;
			numCorrect++;
			if(numCorrect == 3 && level < max_level){//add level checking logic
				level++;
				numCorrect = 0;
				copyLevelWords.clear();
			}
			totalCorrect++;
			if(uniqueWord.length() > max_completed_len){
				max_completed_len = uniqueWord.length();
			}
		}
}
void Game::play(){
	//cout << "level: " << level << " max level: " << max_level << endl;
	if(guesses == 0){
		gameStatus = true;
	}else if(level == max_level && numCorrect == 3){
		cout << " ~ ~ ~ You Win!! ~ ~ ~\n";
		gameStatus = true;
	}else{
		printStats();
		//spit out the word
		string uniqueWord = newWord(level);
		if(uniqueWord == ""){
			gameStatus = true;
		}else{
				promptGuess(uniqueWord);
		}
	}
	
}




