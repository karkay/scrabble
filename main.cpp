#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <algorithm>



#include "game.h"

using namespace std;

int main(int argc, char *argv[]){
	ifstream data;
	ifstream config("config");

	int level = -1;
	int lifeline = -1;
	//creating a pointer
	Game *session;

	//cout << "memory address: " << &session << endl;
	//cout << "memory content: " << session << endl;
	//handles opening or loading files
	if(argc == 2){
		data.open(argv[1]);
		if(data.fail()){
			cerr << "The file " << argv[0] << " cannot be opened.\n";
			exit(1);
		}
		//load words into game class
		session = new Game;
			while(!data.eof()){
				string word;
				getline(data,word);
				session->addWord(word);
			}
		//game.readWords();
		
		

	//no input file argument provided
	}else if(argc == 1){
		if(config){
			stringstream input;
			input << config.rdbuf();
			config.close();

			string line;
			cout << "creating object with no input" << endl;
			while(getline(input,line)){
				istringstream input(line);
				string key;
				if(getline(input,key,'=')){
					string value;
					if(getline(input,value)){
						//cout << key << " : " << value << endl;
						if(key == "level"){
							level = stoi(value)-1;
						}
						else if(key == "lifeline"){
							lifeline = stoi(value);
						}
					}
				}
			}
		}

		
		//cout << "constructing game object with: " << "level: " << level << " | lifeline: " << lifeline << endl;
		session = new Game (level, lifeline);
		//game.readWords();
		//while(!game.isOver()){
		//	game.play();
		//}




	}else{
		cerr << "usage: " << argv[0] << " datafile\n";
		exit(1);
	}

//	game.readWords();
		
	
	session->buildLevels();
	//session->readWords();
	while(!session->isOver()){
		session->play();
	}
	cout << "-----Game Over!------" << endl;
	//session->play();
	session = NULL;
	delete session;




	return 0;
}