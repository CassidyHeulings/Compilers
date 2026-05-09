#include <string>
#include <vector>
#include "Symbols.hpp"
#pragma once

class Node {
public:
    // constructor using name of the node and setting the parent of the node
    Node(const std::string& newName, Node* newParent);
    // add a child to the node children
    void addChild(const std::string& childName);
    // return the address of the pointers of the children of a node
    std::vector<std::unique_ptr<Node>>& getChildren();
    // return the address of the first child
    std::unique_ptr<Node>& getLastChild();
    // return the pointer to the parent node
    Node* getParent();
    // return the name of the node
    const std::string& getName();
    // attatch symbols to node
    void addSymbols();
    // get location of symbol array
    Symbols& getSymbols();
    // make the block for the node
    std::vector<std::string> getBlock();
private:
    // name of node (will double as key in symbol table)
    std::string name;
    // pointer to the parent node
    Node* parent;
    // vector of the pointers to children of the node
    std::vector<std::unique_ptr<Node>> children;
    // pointer to the symbols in the scope
    std::unique_ptr<Symbols> symbols;
};