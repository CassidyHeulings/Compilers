#include <string>
#include "Logger.hpp"
#include "ParseTree.hpp"
#pragma once

// class to hold parser information and functions
class Parser {
public:
    Parser(Logger& loggerInstance, const std::string& processName);
    // get pointers to the addresses of the necessary vectors
    void setValues(std::vector<std::string>& newTokens, std::vector<std::string>& newVals, std::vector<std::string>& newLocs);
    // check if there is more tokens to parse
    void checkForNewTree();
    // to start the parse (create a new tree for each parse)
    void startParse();
    // collect the next piece of input using a pointer
    void nextToken();
    // return the address of the vector of parse trees
    std::vector<std::unique_ptr<ParseTree>>& getTrees();
    // print a tree at the given node location using depth first in order traversal
    void printTree(Node& nodeLoc, int treeLevel);
private:
    // logger instance
    Logger& logger;
    // name of this process for the logger
    std::string name;
    // store the pointers to each tree
    std::vector<std::unique_ptr<ParseTree>> allTrees;
    // pointer to current tree
    ParseTree* currTree = nullptr;
    // pointer to tokens produced by lexer
    std::vector<std::string>* tokens;
    // pointer to token values from the code
    std::vector<std::string>* tokenVals;
    // pointer to token locations from the code
    std::vector<std::string>* tokenLocs;
    // index of token we are on
    int tokenIndex = 0;
    // match the token name function
    void match(std::string expected);
    // parse functions for each token
    void parseProgram();
    void parseBlock();
    void parseStatementList();
    void parseStatement();
    void parsePrintStatement();
    void parseAssignmentStatement();
    void parseVarDecl();
    void parseWhileStatement();
    void parseIfStatement();
    void parseExpr();
    void parseIntExpr();
    void parseStringExpr();
    void parseBooleanExpr();
    void parseId();
    void parseCharList();
    void parseType();
    void parseChar();
    void parseSpace();
    void parseDigit();
    void parseBoolop();
    void parseBoolval();
    void parseIntop();
    // return the expected code of a token
    std::string getTokenValues(std::string token);
};