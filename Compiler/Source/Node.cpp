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

std::vector<std::string> Node::getBlock() {
    std::vector<std::string> box;
    // make a box for the scope node
    box.push_back(" ________________________________ ");
    box.push_back("|");
    // formatting name to be in center
    for (int i = 0; i < 15 - name.length()/2; i++) box.at(1) += " ";
    box.at(1) += name;
    for (int i = 15 + name.length()/2; i <= 30; i++) box.at(1) += " ";
    box.at(1) += "|";
    box.push_back("|  Name   Type  isUsed  isInit   |");
    // keep track of what number index we are on in vector
    int index = 3;
    // get symbols in scope
    std::vector<char> ids = symbols->getSymbols();
    for (char id : ids) {
        box.push_back("| [  ");
        // id name
        box.at(index) += id;
        box.at(index) += ", ";
        // id type
        box.at(index) += symbols->getType(id) + ",";
        // formatting for isUsed
        for (int j = symbols->getType(id).length(); j <= 6; j++) 
                box.at(index) += " ";
        // id isUsed
        box.at(index) += (symbols->getUsed(id) ? "true" : "false");
        box.at(index) += ",";
        // formatting for isInit
        if (symbols->getUsed(id)) box.at(index) += "   ";
        else box.at(index) += "  ";
        // id isInit
        box.at(index) += (symbols->getInit(id) ? "true" : "false");
        // more formatting
        if (symbols->getInit(id)) box.at(index) += "  ]  |";
        else box.at(index) += " ]  |";
        // increase index of vector
        index++;
    }
    box.push_back("|________________________________|");
    return box;
}