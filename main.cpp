#include <iostream>
#include <string>

#include "wordle.h"

int main(int argc, char *argv[]) {
	std::string green = argv[1];
	std::string yellow[5] = {argv[2],argv[3],argv[4],argv[5],argv[6]};
	std::string tried = argv[7];
	wordle wordle(green, yellow, tried);
	std::cout << wordle.guess() << '\n';
	return 0;
}
