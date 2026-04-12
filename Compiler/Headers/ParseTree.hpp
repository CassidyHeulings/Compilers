#pragma once
#include "Node.hpp"

class ParseTree {
public:
    // constructor
    ParseTree();
    // add a child to the tree
    bool addChild(const std::string& newName);
    // TODO retrive a pointer to the next child node from the tree
    //std::unique_ptr<Node> retrieveChildren(const std::string& child);
    // retrive the pointer to the parent node from the tree
    Node* retrieveParent();
private:
    // root node of the tree
    Node root;
    // pointer to the current node we are on
    Node* currNode;
};