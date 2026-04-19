// language
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
using namespace std;
// headers
#include "../Headers/ErrorHandler.hpp"
#include "../Headers/Logger.hpp"
#include "../Headers/Lexer.hpp"
#include "../Headers/Parser.hpp"
#include "../Headers/Semantic.hpp"

// TODO make printing both trees pretty again
// TODO check for functions/vars not being used
// TODO do one program at a time
// TODO log errors and warnings for each process

int main() {
	/* ===== CONSTRUCT CLASS INSTANCES ===== */
	std::string currStage = "Initialization"; // current part of the compiler we are on for logging
	ErrorHandler errorHandler;
	Logger logger(errorHandler, true, false); // errorHandler instance, debugger on, tester on
	logger.startProcess(currStage);
	logger.info(currStage, "Initializing compiler.");
	// initialize each part of compiler
	Lexer lexer(logger, "Lexer");
	logger.debug(currStage, "Lexer is ready.");
	Parser parser(logger, "Parser");
	logger.debug(currStage, "Parser is ready.");
	Semantic semantic(logger, "Semantic Analysis");
	logger.debug(currStage, "Semantic analyzer is ready.");
	logger.endProcess(currStage);


	/* ===== COLLECT INPUT ===== */
	currStage = "Reading Input";
	logger.startProcess(currStage);
	std::ifstream inputFile("Compiler/Input.txt");
	std::string line; // singular line in file
	std::string programs; // string of the input

	// make sure text file is found
	if (!inputFile.is_open()) {
		logger.info(currStage, "Error: Input.txt not found.");
		return 1;
	}
	logger.info(currStage, "Start of input reading.");
	// read the file
	while (std::getline(inputFile, line)) {
		programs += line + "\n"; // keeps track of lines
	}

	logger.debug(currStage, "Creating input information.");
	// get the character length of input
	int progamLength = programs.length();
	// log input information
	logger.test(currStage, "Input: " + programs);
	logger.test(currStage, "input length: " + to_string(progamLength));
	// close file stream
	inputFile.close();
	logger.endProcess(currStage);


	/* === LEXER === */
	currStage = "Lexer"; // change the process to lexer
	logger.startProcess(currStage);
	int currCharNum = 0; // current character location we are on in input
	char currChar; // current character we are on
	char nextChar; // next character to look at
	int lineCount = 1; // track which line we are on
	int charCount = 1; // track which character number we are on
	std::string tokenName = ""; // stores the token
	std::string tokenVal = ""; // stores the value of the token
	bool isComment = false; // true when we are looking in a comment
	std::string commentStart = ""; // location of comment start, used for comment close warning
	std::vector<std::string> tokens; // stores the tokens
	std::vector<std::string> tokenVals; // stores the token values 
	std::vector<std::string> tokenLocs; // stores the locations of tokens in input

	logger.info(currStage, "Starting lexing.");
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
				currCharNum++;
				continue;
			}
			// if we are not in quotes ignore spaces
			if (currChar == ' ' && !lexer.getIsQuotes()) {
				currCharNum++;
				continue; // move to next char
			}
		}

		// check if starting a comment
		if (currChar == '/' && nextChar == '*') {
			isComment = true;
			commentStart = "[" + to_string(lineCount) + ":" + to_string(charCount - 1) + "]"; // - 1 for length of comment syntax
		}

		// ignore comments
		if (isComment) {
			// end the comment
			if (currChar == '*' && nextChar == '/') {
				isComment = false; // no longer in a comment
				// skip past the next char /
				currCharNum++;
				charCount++;
			}
			currCharNum++;
			continue; // move to next char
		}

		// debugging
		std::string debug = "Current character: ";
		debug += currChar;
		std::string debug1 = "Next character: ";
		debug1 += nextChar;
		logger.test(currStage, debug);
		logger.test(currStage, debug1);

		// if the character completes a token
		if (lexer.isCompleteToken(currChar, nextChar)) {
			// get the token
			tokenName = lexer.getToken();
			if (tokenName == "TRYAGAIN") {
				// move back to starting char of incomplete token
				currCharNum = currCharNum - lexer.getBufferLength();
				charCount = charCount - lexer.getBufferLength();
			}
			else {
				// store the token name
				tokens.push_back(tokenName);
				// get the value of the token
				tokenVal = lexer.getTokenValue();
				// store the token value
				tokenVals.push_back(tokenVal);
				// store the location of the token
				tokenLocs.push_back("[" + to_string(lineCount) + ":" + to_string(charCount) + "]");
				// log the token and location
				logger.debug(currStage, "\033[36m" + tokenName + "\033[0m [ " + tokenVal + " ] found at [" + to_string(lineCount) + ":" + to_string(charCount - tokenVal.length()) + "]");
				// reset the max buffer
				lexer.resetMaxBuffer();
			}
			lexer.clearBuffer(); // clear buffer for next token
			lexer.resetState(); // reset the state for the next token
		}

		// move to next char
		currCharNum++;
		logger.test(currStage, "Character num: " + to_string(currCharNum));
	}

	// send a warning if the comment was never finished
	if (isComment) {
		logger.warning(currStage, 0, "Comment started at " + commentStart + " -> Close comment using */");
	}

	// check if there is any tokens at all
	if (tokens.size() > 0) {
		// TODO move this to next stage as error
		// send a warning if there was no EOP symbol
		if (tokenVals.at(tokenVals.size() - 1) != "$") {
			logger.warning(currStage, 1, "End each program with EOP symbol $");
		}
	}
	else {
		// send a warning if empty code
		logger.warning(currStage, 2, "Add code to Input.txt file, ensure it is not commented out with /* */");
	}

	// decide if end of program
	if (logger.endProcess(currStage)) {
		logger.endProgram();
		return 1; 
	}


	/* ===== PARSER ===== */
	currStage = "Parser"; // change the process to lexer
	logger.startProcess(currStage);
	logger.info(currStage, "Starting parser.");

	// send the lexer values to the parser
	parser.setValues(tokens, tokenVals, tokenLocs);
	parser.startParse();
	// when parse is done, get the location where all the trees are stored
	std::vector<std::unique_ptr<ParseTree>>& parseTrees = parser.getTrees();

	// print out the parse trees using a depth first in order traversal
	int progNum = 1;
	for (std::__1::unique_ptr<ParseTree>& tree : parseTrees) {
		logger.debug(currStage, "\033[35mProgram #" + to_string(progNum) + "\033[0m: ");
		// get the root node of the tree to start at level -1 of the tree (root does not print)
		parser.printTree(tree->retrieveRoot(), -1);
		progNum++;
	}

	// if any errors occured, end the program
	if (logger.endProcess(currStage)) {
		// this will only happen if debug is on, as parse errors are caught during process
		logger.warning(currStage, 3, "Fix first error and try again.");
		logger.endProgram();
		return 1; 
	}


	/* ===== SEMANTIC ANALYSIS ===== */
	currStage = "Semantic Analysis"; // set the new stage
	logger.startProcess(currStage);
	logger.info(currStage, "Starting semantic analysis.");

	// turn each cst into a ast
	for (std::__1::unique_ptr<ParseTree>& tree : parseTrees) {
		semantic.createAst(tree);
	}

	// get the vector of abstract syntax trees
	std::vector<std::unique_ptr<AbstractTree>>& abstractTrees = semantic.getTrees();

	// print each tree
	progNum = 1;
	for (std::__1::unique_ptr<AbstractTree>& tree : abstractTrees) {
		logger.debug(currStage, "\033[35mProgram #" + to_string(progNum) + "\033[0m: ");
		semantic.printTree(tree->retrieveRoot(), -1);
		progNum++;
	}

	// if any errors occured, end the program
	if (logger.endProcess(currStage)) {
		logger.endProgram();
		return 1; 
	}


	/* ===== NEXT STAGE ===== */


	/* ===== END OF COMPILE ===== */
	// print end of program line
	logger.endProgram();
	return 0;
}