#include "wordle.h"
#include "answers.h"
#include <iostream>
#include <regex>

wordle::wordle(std::string greenDef, std::string yellowDef[5], std::string triedDef) {
	for (size_t i = 0; i < 5; i++) {
		if (i>=greenDef.length()) {
			this->green[i] = ' ';
		} else {
			this->green[i] = tolower(greenDef[i]);
		}
	}
	for (size_t column = 0; column < 5; column++) {
		for (size_t i = 0; i < 5; i++) {
			if (i>=yellowDef[column].length()) {
				yellow[column][i] = ' ';
			} else {
				yellow[column][i] = tolower(yellowDef[column][i]);
			}
		}
	}
	this->addTried(triedDef);
}
wordle::wordle() {
	std::string yellowDef[5] = {"","","","",""};
	wordle("",yellowDef,"");
}
void wordle::addGreen(char add[5]) {
}
void wordle::addYellow(char add[5]) {
}
void wordle::addTried(std::string newTried) {
	for (size_t i = 0; i < newTried.length(); i++) {
		char letter = tolower(newTried[i]);
		int position = letter - 'a';
		tried[position] = true;
	}
}
std::string wordle::getPossibleAnswers() {
	// First, eliminate the words it can't be
	std::string possibleAnswers = "";
	{
		std::string match = "\n";
		for (size_t column = 0; column < 5; column++) {
			if (green[column]==' ') {
				match += "[^\n";
				for (size_t i = 0; i < 5; i++) {
					if (yellow[column][i]!=' ') {
						match += yellow[column][i];
					}
				}
				match += "]";
			} else {
				match += green[column];
			}
		}
		std::regex regex(match);
		std::string s = answers;
		auto begin = std::sregex_iterator(s.begin(), s.end(), regex);
		auto end = std::sregex_iterator();
		std::cout << "Found " << std::distance(begin,end) <<'\n';
		for (std::sregex_iterator i = begin; i != end; ++i) {
			std::smatch match = *i;
			std::string matchString = match.str();
			possibleAnswers += matchString;
		}
	}
	// Second, eliminate what we've already tried
	std::string match = "\n[^";
	for (size_t i = 0; i < 26; i++) {
		if (tried[i]==true) {
			match += ('a' + i);
		}
	}
	match += "]{5}\n";
	std::regex regex(match);
	std::string s = possibleAnswers;
	auto begin = std::sregex_iterator(s.begin(), s.end(), regex);
	auto end = std::sregex_iterator();
	std::string possibleArray[std::distance(begin,end)];
	std::cout << "Found " << std::distance(begin,end) <<'\n';
	int matchNum = 0;
	for (std::sregex_iterator i = begin; i != end; ++i) {
		std::smatch match = *i;
		std::string matchString = match.str();
		possibleArray[matchNum] = matchString;
		matchNum ++;
	}
	// Next, check each word against our yellow letters
	bool yellowLetters[26] = {false};
	for (size_t i = 0; i < 5; i++) {
		for (size_t j = 0; j < 5; j++) {
			char letter = yellow[i][j];
			if (letter != ' ') {
				int position = letter - 'a';
				yellowLetters[position] = true;
			}
		}
	}
	std::string yellowLettersRegex = "";
	for (size_t i = 0; i < 26; i++) {
		std::cout << yellowLetters[i];
		if (yellowLetters[i] == true) {
			yellowLettersRegex += 'a' + i;
		}
	}
	std::cout << '\n';
	possibleAnswers = "";
	for (size_t i = 0; i < matchNum; i++) {
		bool failed = false;
		for (size_t letter = 0; letter < yellowLettersRegex.length(); letter++) {
			if (possibleArray[i].find(yellowLettersRegex[letter]) == std::string::npos) {
				failed = true;
			}
		}
		if (failed == false) {
			possibleAnswers += possibleArray[i];
		}
	}
	return possibleAnswers;
}
int getCharCount(std::string string, char character) {
	int count = 0;
	for (size_t i = 0; i < string.length(); i++) {
		if (string[i]==character) {
			count ++;
		}
	}
	return count;
}
std::string wordle::guess() {

	std::string possibleAnswers = getPossibleAnswers();
	if (possibleAnswers=="") {
		return "";
	}
	char highestChars[] = {'.','.','.','.','.'};
	int highestCount[5] = {0};
	for (char letter = 'a'; letter < ('a' + 26); letter++) {
		int count = getCharCount(possibleAnswers, letter);
		if (count > highestCount[0]) {
			for (size_t i = 0; i < 4; i++) {
				if (highestCount[i+1] > count) {
					break;
				}
				highestChars[i] = highestChars[i+1];
				highestCount[i] = highestCount[i+1];
				highestChars[i+1] = letter;
				highestCount[i+1] = count;
			}
		}
	}
	std::string guesses = possibleAnswers;
	for (int i = 4; i >= 0; --i) {
		std::string s = "";
		std::string m = "\n[a-z]*";
		m += highestChars[i];
		m += "[a-z]*\n";
		std::cout << i << m;
		std::regex regex(m);
		auto begin = std::sregex_iterator(guesses.begin(), guesses.end(), regex);
		auto end = std::sregex_iterator();
		for (std::sregex_iterator ri = begin; ri != end; ++ri) {
			std::smatch match = *ri;
			std::string matchString = match.str();
			s += matchString;
		}
		if (s=="") {
			break;
		}
		guesses = s;
	}
	return guesses;

}
