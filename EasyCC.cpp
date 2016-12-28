#include "lexical/Lexical.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace ecc;

int main() {
	Lexical lexical(
			"/home/amir/github/EasyCC-CPP/lexical/state_machine.json",
			"/home/amir/github/EasyCC-CPP/lexical/config.json",
            "/home/amir/github/EasyCC-CPP/lexical/errors.json");

	std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
	std::vector<std::string> errorMessages;
	lexical.generateLexicalTokens("/home/amir/github/EasyCC-CPP/input.txt", lexicalTokens, errorMessages);

	for(auto token : lexicalTokens)
		cout << *token << endl;

	for(auto token : errorMessages)
		cerr << token << endl;

    if(errorMessages.size() != 0) {
        // A lexical error exist, exit
        cerr << "Exiting program with code 1" << endl;
        return 1;
    }

	return 0;
}
