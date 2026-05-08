#include "../Headers/Node.hpp"

Node::Node(const std::string& newName, Node* newParent) 
    : name(newName), parent(newParent) {
    // assign parent and name to node
}

// add a child to the children vector of the node that called the function
void Node::addChild(const std::string& childName) {
    children.push_back(std::make_unique<Node>(childName, this));
}

std::vector<std::unique_ptr<Node>>& Node::getChildren() {
    return children;
}

std::unique_ptr<Node>& Node::getLastChild() {
    return children.back();
}

Node* Node::getParent() {
    return parent;
}

const std::string& Node::getName() {
    return name;
}

void Node::addSymbols() {
    symbols = std::make_unique<Symbols>();
}

Symbols& Node::getSymbols() {
    return *symbols;
}