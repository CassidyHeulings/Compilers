#include "Node.hpp"
#pragma once

class ParseTree {
public:
    // constructor
    ParseTree();
    // move the current node pointer up the tree by 1 level
    bool moveUpTree();
    // add a child to the tree
    void addChild(const std::string& newName);
    // retrieve location of the root node of the tree
    Node& retrieveRoot();
private:
    // root node of the tree
    Node root;
    // pointer to the current node we are on
    Node* currNode;
};