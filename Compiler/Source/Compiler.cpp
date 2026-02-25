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
	std::string currStage; // current part of the compiler we are on for logging

	/* === COLLECT INPUT === */
	currStage = "Reading Input";
	logger.info(currStage, "Reading programs.");
	std::ifstream inputFile("Compiler/Input.txt");
	std::string line; // singular line in file
	std::string programs; // string of the input

	// make sure text file is found
	if (!inputFile.is_open()) {
		logger.info(currStage, "Error: Input.txt not found.");
		return 1;
	}
	// read the file
	while (std::getline(inputFile, line)) {
		programs += "\n" + line; // keeps track of lines
	}

	// get the character length of input
	int progamLength = programs.length();
	// log input information
	logger.debug(currStage, "Input: " + programs);
	logger.debug(currStage, "input length: " + to_string(progamLength));
	// close file stream
	inputFile.close();
	logger.debug(currStage, "Input Finished Reading");

	/* === LEXER === */
	currStage = "Lexer"; // change the process to lexer
	int currCharNum = 0; // current character location we are on in input
	char currChar; // current character we are on
	int lineCount = 0; // track which line we are on
	int charCount = 0; // track which character number we are on
	std::string tokenName = ""; // stores the token
	std::string tokenVal = ""; // stores the value of the token

	// loop through each character in the program
	while (currCharNum < progamLength) {
		currChar = programs[currCharNum]; // set the value of the character
		charCount++; // increase character count in line

		// ignore white space
		if (isspace(currChar)) {
			if (currChar == '\n') {
				lineCount++; // increase line count
				charCount = 1; // reset character count in line
			}
			currCharNum++;
			continue;
		}

		// debugging
		std::string debug = "Current character: ";
		debug += currChar;
		logger.debug(currStage, debug);

		// if the character completes a token
		if (lexer.isCompleteToken(currChar)) {
			// get the token
			tokenName = lexer.getToken();
			// get the value of the token
			tokenVal = lexer.getTokenValue();
			// log the token
			logger.debug(currStage, tokenName + " [" + tokenVal + "] found at [" + to_string(lineCount) + ":" + to_string(charCount - tokenName.length()) + "]");
			// store the token
			// log the token and location
			lexer.clearBuffer(); // clear buffer for next token
			lexer.resetState(); // reset the state for the next token
		}
		// move to next char
		currCharNum++;
		logger.debug(currStage, "Character num: " + to_string(currCharNum));
	}


	return 0;
}