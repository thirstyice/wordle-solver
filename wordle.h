#include <string>

#ifndef WORDLE
#define WORDLE

class wordle {
private:
	char green[5];
	char yellow[5][5];
	bool tried[26] = {false};


public:
	wordle(std::string, std::string[5], std::string);
	wordle();
	void addGreen(char[5]);
	void addYellow(char[5]);
	void addTried(std::string);
	int getNumPossible();
	std::string getPossibleAnswers();
	std::string guess();
};

#endif
