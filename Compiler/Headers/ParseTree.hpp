#pragma once
#include "Node.hpp"

class ParseTree {
public:
    // constructor
    ParseTree();
    // move the current node pointer up the tree by 1 level
    void moveUpTree();
    // add a child to the tree
    void addChild(const std::string& newName);
    // retrive the pointer to the parent node from the tree
    Node* retrieveParent();
private:
    // root node of the tree
    Node root;
    // pointer to the current node we are on
    Node* currNode;
};