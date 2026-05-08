#include <string>
#include "Logger.hpp"
#include "Tree.hpp"
#pragma once

class SymbolTable {
public:
    SymbolTable(Logger& loggerInstance, const std::string& processName);
    // create the symbol table based on the ast
    void createTable(std::__1::unique_ptr<Tree>& ast);
    // print the symbol table
    void printTable(Node& nodeLoc, int treeLevel);
    // get symbol table
    std::unique_ptr<Tree>& getTable();
private:
    // logger instance
    Logger& logger;
    // name of this process for the logger
    std::string name;
    // pointer to the table
    std::unique_ptr<Tree> table = nullptr;
    // add to the symbol table
    void buildTable(Node& nodeLoc, int treeLevel);
};