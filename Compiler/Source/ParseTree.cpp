#include "ParseTree.hpp"

ParseTree::ParseTree() : root("Root", nullptr), currNode(&root) {
    // the root node will have a null parent node
    // it is the root of the tree
    // set the current node pointer to the address of the root node
    // this is the node we start on
}

bool ParseTree::addChild(const std::string& newName) {
    currNode->addChild(newName);
}

/* std::unique_ptr<Node> ParseTree::retrieveChildren() {
    // get children vector of current node and choose the first one
    // TODO we will remove the children as we cycle through
    std::vector<std::unique_ptr<Node>> children = currNode->getChildren();
    return children;
} */

Node* ParseTree::retrieveParent() {
    // get the pointer to the parent of the node we are on
    currNode->getParent();
}