#include <string>
#include "Logger.hpp"
#include "Node.hpp"
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
    // store the root of each tree
    Node allRoots[];
    // a function to add node
    // a function to move up the tree to parent node
    // a function to match the token
    // a function for each token possibility

};