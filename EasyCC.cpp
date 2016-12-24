#include "lexical/Lexical.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ecc;

int main() {
	Lexical lexical;
	std::shared_ptr<Graph> graph =
			lexical.buildGraph("/home/amir/github/EasyCC-CPP/lexical/state_machine.json");
	return 0;
}
