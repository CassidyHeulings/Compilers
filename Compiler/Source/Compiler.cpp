// language
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;
// headers
#include "../Headers/Logger.hpp"
#include "../Headers/Lexer.hpp"

int main() {
	/* === CONSTRUCT CLASS INSTANCES === */
	Logger logger(true);
	Lexer lexer;

	/* === COLLECT INPUT === */
	logger.info("Reading programs.");
	std::ifstream inputFile("Compiler/Input.txt");
	std::string line; // singular line in file
	std::string programs; // string of the input

	// make sure text file is found
	if (!inputFile.is_open()) {
		logger.info("Error: Input.txt not found.");
		return 1;
	}
	// read the file
	while (std::getline(inputFile, line)) {
		programs += "\n" + line; // keeps track of lines
	}

	// get the character length of input
	int progamLength = programs.length();
	// log input information
	logger.debug("Input: " + programs);
	logger.debug("input length: " + to_string(progamLength));
	// close file stream
	inputFile.close();

	/* === LEXER === */
	int currCharNum = 0; // current character location we are on in input
	char currChar; // current character we are on

	// loop through each character in the program
	while (currCharNum < progamLength) {
		// set the value of the character
		currChar = programs[currCharNum];

		// ignore white space
		if (isspace(currChar)) {
			currCharNum++;
			continue;
		}

		// debugging
		std::string debug = "Current character: ";
		debug += currChar;
		logger.debug(debug);

		// if the character completes a token
		if (lexer.isCompleteToken(currChar)) {
			// get the token
			logger.debug(lexer.getToken()); // log the token
			// store the token
			// log the token and location
			lexer.clearBuffer(); // clear buffer for next token
			lexer.resetState(); // reset the state for the next token
		}
		// move to next char
		currCharNum++;
		logger.debug("Character num: " + to_string(currCharNum));
	}


	return 0;
}