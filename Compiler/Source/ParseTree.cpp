#include "../Headers/ParseTree.hpp"

ParseTree::ParseTree() : root("Root", nullptr), currNode(&root) {
    // the root node will have a null parent node
    // it is the root of the tree
    // set the current node pointer to the address of the root node
    // this is the node we start on
}

void ParseTree::moveUpTree() {
    currNode = currNode->getParent();
}

void ParseTree::addChild(const std::string& newName) {
    currNode->addChild(newName);
    // set the current node to this new child node
    currNode = currNode->getLastChild().get();
}

Node& ParseTree::retrieveRoot() {
    return root;
}