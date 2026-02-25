// language
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;
// headers
#include "../Headers/ErrorHandler.hpp"
#include "../Headers/Logger.hpp"
#include "../Headers/Lexer.hpp"

int main() {
	/* === CONSTRUCT CLASS INSTANCES === */
	ErrorHandler errorHandler;
	Logger logger(errorHandler, true);
	Lexer lexer(logger, "Lexer");
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
		programs += line + "\n"; // keeps track of lines
	}

	// get the character length of input
	int progamLength = programs.length();
	// log input information
	//logger.debug(currStage, "Input: " + programs);
	//logger.debug(currStage, "input length: " + to_string(progamLength));
	// close file stream
	inputFile.close();
	logger.debug(currStage, "Input Finished Reading");

	/* === LEXER === */
	currStage = "Lexer"; // change the process to lexer
	int currCharNum = 0; // current character location we are on in input
	char currChar; // current character we are on
	char nextChar; // next character to look at
	int lineCount = 1; // track which line we are on
	int charCount = 1; // track which character number we are on
	std::string tokenName = ""; // stores the token
	std::string tokenVal = ""; // stores the value of the token

	// loop through each character in the program
	while (currCharNum < progamLength) {
		currChar = programs[currCharNum]; // set the value of the character
		nextChar = programs[currCharNum + 1]; // set value of the next character
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
		std::string debug1 = "Next character: ";
		debug1 += nextChar;
		logger.debug(currStage, debug);
		logger.debug(currStage, debug1);

		// if the character completes a token
		if (lexer.isCompleteToken(currChar, nextChar)) {
			// get the token
			tokenName = lexer.getToken();
			// get the value of the token
			tokenVal = lexer.getTokenValue();
			// log the token and location
			logger.debug(currStage, tokenName + " [" + tokenVal + "] found at [" + to_string(lineCount) + ":" + to_string(charCount - tokenVal.length()) + "]");
			// store the token
			lexer.clearBuffer(); // clear buffer for next token
			lexer.resetState(); // reset the state for the next token
		}

		// move to next char
		currCharNum++;
		logger.debug(currStage, "Character num: " + to_string(currCharNum));
	}


	return 0;
}