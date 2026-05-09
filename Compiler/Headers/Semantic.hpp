#include "Logger.hpp"
#include "Tree.hpp"
#pragma once

// class for semantic analysis creating the AST
class Semantic {
public:
    Semantic(Logger& loggerInstance, const std::string& processName);
    // start a new ast using a cst
    void createAst(std::__1::unique_ptr<Tree>& cst);
    // print out the ast
    void printTree(Node& nodeLoc, int treeLevel);
    // get ast tree
    std::unique_ptr<Tree>& getTree();
    // create the symbol table based on the ast
    void createTable(std::__1::unique_ptr<Tree>& ast);
    // get symbol table
    std::unique_ptr<Tree>& getTable();
    // print the symbol table
    void printTable(Node& nodeLoc);
private:
    // logger instance
    Logger& logger;
    // name of the process for the logger
    std::string name;
    // pointer to the tree
    std::unique_ptr<Tree> tree = nullptr;
    // used to turn chars in a charlist into a full string for ast
    std::string currString;
    // pointer to the symbol table
    std::unique_ptr<Tree> table = nullptr;
    // string that is the symbol table
    std::vector<std::string> fullTable;
    // add children to the tree
    void buildAst(Node& currNode);
    // add children to the symbol table
    void buildTable(Node& nodeLoc, int treeLevel);
    // make the table ready for printing
    void createPrintTable(Node& nodeLoc, int row, int column);
    // expand the table rows to be the size of the row num
    void expandRows(int row);
    // expand the table columns to be the size of the column num
    void expandColumns(int row, int column);
};