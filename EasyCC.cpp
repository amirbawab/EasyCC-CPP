#include "lexical/Lexical.h"
#include <iostream>
#include <vector>

using namespace std;
using namespace ecc;

int main() {
	Lexical lexical(
			"/home/amir/github/EasyCC-CPP/lexical/state_machine.json",
			"/home/amir/github/EasyCC-CPP/lexical/config.json");

	std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
	lexical.generateLexicalTokens("/home/amir/github/EasyCC-CPP/input.txt", lexicalTokens);

	for(auto token : lexicalTokens)
		cout << *token << endl;

	return 0;
}
