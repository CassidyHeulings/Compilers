#include "ParseTree.hpp"
#pragma once

// symbol table that will hold values for the AST
// extends the parse tree as we still want the tree and node functionality
class AbstractTree : ParseTree {
public:
    // constructor
    AbstractTree();
private:
    // root node of the tree
    Node root;
    // pointer to the current node we are on
    Node* currNode;
};