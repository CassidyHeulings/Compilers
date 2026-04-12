#include "Node.hpp"

Node::Node(const std::string& newName, Node* newParent) 
    : name(newName), parent(newParent) {
    // assign parent and name to node
}

// add a child to the children vector of the node that called the function
void Node::addChild(const std::string& childName) {
    std::unique_ptr<Node> newChild = std::make_unique<Node>(childName, this);
    children.push_back(newChild);
}

std::vector<std::unique_ptr<Node>>& Node::getChildren() {
    return children;
}

std::unique_ptr<Node>& Node::getChild() {
    return children.front();
}

Node* Node::getParent() {
    return parent;
}