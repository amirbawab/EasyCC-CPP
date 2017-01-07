#include "../lexical/Lexical.h"
#include "../syntax/Syntax.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;
using namespace ecc;

int main() {

    // Lexical analysis phase
    Lexical lexical(
			"resources/src/state_machine.json",
			"resources/src/config.json",
            "resources/src/errors.json");

	std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
	std::vector<std::string> errorMessages;
	lexical.generateLexicalTokens("resources/src/input.txt", lexicalTokens, errorMessages);

    // Logging
	for(auto token : lexicalTokens)
		cout << *token << endl;
	for(auto token : errorMessages)
		cerr << token << endl;
    if(errorMessages.size() != 0) {
        // A lexical error exist, exit
        cerr << "Exiting program with code 1" << endl;
        return 1;
    }

    // Syntax analysis phase
    Syntax syntax("resources/src/grammar.txt");

	return 0;
}
