#include "lexical/Lexical.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace ecc;

int main() {
	Lexical lexical(
			"/home/amir/github/EasyCC-CPP/lexical/state_machine.json",
			"/home/amir/github/EasyCC-CPP/lexical/config.json",
            "/home/amir/github/EasyCC-CPP/lexical/errors.json");

	std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
	std::vector<std::shared_ptr<LexicalToken>> errorTokens;
	lexical.generateLexicalTokens("/home/amir/github/EasyCC-CPP/input.txt", lexicalTokens, errorTokens);

	for(auto token : lexicalTokens)
		cout << *token << endl;

	for(auto token : errorTokens)
		cout << *token << endl;

	return 0;
}
