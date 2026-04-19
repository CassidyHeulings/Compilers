#include "ParseTree.hpp"
#pragma once

// abstract syntax tree
// extends the parse tree as we still want the tree and node functionality
// own class for name and organization
class AbstractTree : public ParseTree {
public:
    // constructor
    AbstractTree();
private:
    // root node of the tree
    Node root;
    // pointer to the current node we are on
    Node* currNode;
};