// language
#include <iostream>
#include <fstream>
#include <cctype>
#include <vector>
#include <sstream>
using namespace std;
// headers
#include "../Headers/Logger.hpp"
#include "../Headers/Lexer.hpp"
#include "../Headers/Parser.hpp"
#include "../Headers/Semantic.hpp"

// TODO make printing both trees pretty again
// TODO make sure all necessary parts are in AST
// use warning code 3 if printing cst after an error
// make input file, test var, and debug var part of the command

void compile(std::string program, int progNum, std::string currStage, Logger& logger, Lexer& lexer, Parser& parser, Semantic& semantic) {
	// log the program number we are on
	logger.startProgram(progNum);

	/* === PREPARE COMPILE === */
	currStage = "Preparing Compile";
	logger.startProcess(currStage);
	logger.debug(currStage, "Compiling \033[35mProgram #" + to_string(progNum) + "\033[0m.");
	// get the character length of input
	int programLength = program.length();
	// log input information
	logger.test(currStage, "Input: " + program);
	logger.test(currStage, "input length: " + to_string(programLength));
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
	std::string commentStart = ""; // location of comment start, used for comment close warning and parsing
	std::vector<std::string> tokens; // stores the tokens
	std::vector<std::string> tokenVals; // stores the token values 
	std::vector<std::string> tokenLocs; // stores the locations of tokens in input

	logger.info(currStage, "Starting lexing.");
	lexer.newProgram();

	// loop through each character in the program
	while (currCharNum < programLength) {
		currChar = program[currCharNum]; // set the value of the character
		nextChar = program[currCharNum + 1]; // set value of the next character
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

	// if there was never a comment created, make it the location of the final piece of code
	if (commentStart == "")
		// + 1 for location after final char
		commentStart = "[" + to_string(lineCount) + ":" + to_string(charCount + 1) + "]";

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

	// if any errors occured, end the programs process
	if (logger.endProcess(currStage)) {
		return; 
	}


	/* ===== PARSER ===== */
	currStage = "Parser"; // change the process to lexer
	logger.startProcess(currStage);
	logger.info(currStage, "Starting parser.");

	// mitigate out of range vector checking token vector at token index
    // will check next tokens, if code provided abrubtly ends, a token is needed that matches nothing
	// if recursion tries to check a match after the final char, tokens->at(tokenIndex) will fail
    // without another token to check, we will be increasing the tokenIndex to a size that is greater than the length of the vectors
    // if we dont increase the token index, recursion will keep checking the same token
    tokens.push_back("Error");
    tokenVals.push_back("Nothing");
    tokenLocs.push_back(commentStart); // location of last start of comment

	// send the lexer values to the parser
	logger.test(currStage, "Setting values.");
	parser.setValues(tokens, tokenVals, tokenLocs);
	logger.test(currStage, "Starting parse.");
	parser.startParse();

	// when parse is done, get the location where the tree is stored
	logger.test(currStage, "Getting parse tree.");
	std::unique_ptr<Tree>& parseTree = parser.getTree();

	// print out the parse tree using a depth first in order traversal
	logger.test(currStage, "Printing parse tree.");
	parser.printTree(parseTree->retrieveRoot(), -1);

	// if any errors occured, end the programs process
	if (logger.endProcess(currStage)) {
		return; 
	}


	/* ===== SEMANTIC ANALYSIS ===== */
	currStage = "Semantic Analysis"; // set the new stage
	logger.startProcess(currStage);
	logger.info(currStage, "Starting semantic analysis.");

	// turn the cst into a ast
	semantic.createAst(parseTree);
	// get the abstract syntax tree
	std::unique_ptr<Tree>& abstractTree = semantic.getTree();
	// print the tree
	semantic.printTree(abstractTree->retrieveRoot(), -1);

	// if any errors occured, end the programs process
	if (logger.endProcess(currStage)) {
		return; 
	}


	/* ===== NEXT STAGE ===== */


	/* ===== END OF COMPILE ===== */
}

int main() {
	/* ===== CONSTRUCT CLASS INSTANCES ===== */
	std::string currStage = "Initialization"; // current part of the compiler we are on for logging
	Logger logger(true, true); // debugger on, tester on
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

	// remove the final "\n"
	if (!programs.empty())
		programs.pop_back();

	// close file stream
	inputFile.close();

	// seperate each program based on eop
	std::string segment;
	std::vector<std::string> progList;
	std::stringstream progStream(programs);

	while(getline(progStream, segment, '$')) {
		// dont add the end of program symbol if it was never found
		if (progStream.eof())
			progList.push_back(segment);
		else
			progList.push_back(segment + '$');
	}
	// remove the final segment if at least one $ was found
	// the final segment will be empty
	if (progList.size() > 1)
		progList.pop_back();

	if (progList.empty()) {
		// send a warning if empty code
		logger.warning(currStage, 2, "Add code to Input.txt file, ensure it is not commented out with /* */");
	}
	logger.endProcess(currStage);

	for (int i = 0; i < progList.size(); i++) {
		compile(progList[i], i + 1, currStage, logger, lexer, parser, semantic);
	}

	// print end of program line
	logger.endProgram();
	return 0;
}