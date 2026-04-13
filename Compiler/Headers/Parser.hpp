#include <string>
#include "Logger.hpp"
#include "ParseTree.hpp"
#pragma once

// class to hold parser information and functions
class Parser {
public:
    Parser(Logger& loggerInstance, const std::string& processName);
    // get pointers to the addresses of the necessary vectors
    void setValues(std::vector<std::string>& newTokens, std::vector<std::string>& newVals);
    // check if there is more tokens to parse
    void checkForNewTree();
    // to start the parse (create a new tree for each parse)
    void startParse();
    // collect the next piece of input using a pointer
    void nextToken();
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
    // index of token we are on
    int tokenIndex = 0;
    // match the token name function
    void match(std::vector<std::string> expected);
    // match the token and token value
    void matchValue(std::vector<std::string> expectedVal);
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
    bool parseChar();
    bool parseSpace();
    bool parseDigit();
    bool parseBoolop();
    bool parseBoolval();
    bool parseIntop();
    // a function to add node
    // a function to move up the tree to parent node
    // a function to match the token
    // a function for each token possibility

};