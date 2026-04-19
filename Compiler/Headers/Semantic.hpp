#include "Logger.hpp"
#include "Tree.hpp"
#pragma once

// class for semantic analysis creating the AST
class Semantic {
public:
    Semantic(Logger& loggerInstance, const std::string& processName);
    // get pointers to the addresses of the necessary vectors
    //void setValues(std::vector<std::string>& newTokens, std::vector<std::string>& newVals);
    // start a new ast using a cst
    void createAst(std::__1::unique_ptr<Tree>& cst);
    // print out the ast
    void printTree(Node& nodeLoc, int treeLevel);
    // get all ast trees
    std::vector<std::unique_ptr<Tree>>& getTrees();
private:
    // logger instance
    Logger& logger;
    // name of the process for the logger
    std::string name;
    // store the pointers to each tree
    std::vector<std::unique_ptr<Tree>> allTrees;
    // pointer to current tree
    Tree* currTree = nullptr;
    // used to turn chars in a charlist into a full string for ast
    std::string currString = "";
    // add children to the tree
    void buildAst(Node& currNode);
};