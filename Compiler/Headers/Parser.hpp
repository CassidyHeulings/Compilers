#include <string>
#include "Logger.hpp"
//#include "Node.hpp"
#include "ParseTree.hpp"
#pragma once

// class to hold parser information and functions
class Parser {
public:
    Parser(Logger& loggerInstance, const std::string& processName);
    // to start the parse (create a new tree for each parse)
    void startParse();
    // collect the next piece of input using a pointer
    void nextToken();
private:
    // logger instance
    Logger& logger;
    // name of this process for the logger
    std::string name;
    // store the location of each tree
    std::vector<std::unique_ptr<ParseTree>> allTrees;
    ParseTree* currTree = nullptr;
    // match function
    void match(const std::string& expected);
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
    bool parseType();
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