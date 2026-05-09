#include "Node.hpp"
#pragma once

class Tree {
public:
    // constructor
    Tree();
    // move the current node pointer up the tree by 1 level
    bool moveUpTree();
    // add a child to the tree
    void addChild(const std::string& newName);
    // retrieve location of the root node of the tree
    Node& retrieveRoot();
    // add a child to the tree with symbols
    void addSymbolsToNode();
    // get location of symbol array of current node
    Symbols& getSymbols();
    // get the current node
    Node& getCurr();
private:
    // root node of the tree
    Node root;
    // pointer to the current node we are on
    Node* currNode;
};