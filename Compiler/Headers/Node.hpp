#include <string>
#include <vector>
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
private:
    // name of node
    std::string name;
    // pointer to the parent node
    Node* parent;
    // vector of the pointers to children of the node
    std::vector<std::unique_ptr<Node>> children;
    
};