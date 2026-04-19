#include "Logger.hpp"
#include "AbstractTree.hpp"
#pragma once

// class for semantic analysis creating the AST
class Semantic {
public:
    Semantic(Logger& loggerInstance, const std::string& processName);
    // get pointers to the addresses of the necessary vectors
    //void setValues(std::vector<std::string>& newTokens, std::vector<std::string>& newVals);
    // start a new ast using a cst
    void createAst(std::__1::unique_ptr<ParseTree>& cst);
    // add children to the tree
    void buildAst(Node& currNode);
    // print out the ast
    void printTree(Node& nodeLoc, int treeLevel);
    // get all ast trees
    std::vector<std::unique_ptr<AbstractTree>>& getTrees();
private:
    // logger instance
    Logger& logger;
    // name of the process for the logger
    std::string name;
    // store the pointers to each tree
    std::vector<std::unique_ptr<AbstractTree>> allTrees;
    // pointer to current tree
    AbstractTree* currTree = nullptr;
    // pointer to tokens produced by lexer
    //std::vector<std::string>* tokens;
    // pointer to token values from the code
    //std::vector<std::string>* tokenVals;
    // index of token we are on
    //int tokenIndex = 0;
};