#include "../Headers/Tree.hpp"

Tree::Tree() : root("Root", nullptr), currNode(&root) {
    // the root node will have a null parent node
    // it is the root of the tree
    // set the current node pointer to the address of the root node
    // this is the node we start on
}

bool Tree::moveUpTree() {
    // avoid segmentation fault
    if (currNode->getParent()) {
        currNode = currNode->getParent();
        return true;
    }
    return false;
}

void Tree::addChild(const std::string& newName) {
    currNode->addChild(newName);
    // set the current node to this new child node
    currNode = currNode->getLastChild().get();
}

Node& Tree::retrieveRoot() {
    return root;
}

void Tree::addSymbolsToNode() {
    // attatch symbol array to node
    currNode->addSymbols();
}

Symbols& Tree::getSymbols() {
    return currNode->getSymbols();
}

Node& Tree::getCurr() {
    return *currNode;
}