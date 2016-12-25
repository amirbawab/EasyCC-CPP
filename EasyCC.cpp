#include "lexical/Lexical.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ecc;

int main() {
	Lexical lexical(
			"/home/amir/github/EasyCC-CPP/lexical/state_machine.json",
			"/home/amir/github/EasyCC-CPP/lexical/config.json");
	return 0;
}
