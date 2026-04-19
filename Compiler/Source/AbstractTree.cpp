#include "../Headers/AbstractTree.hpp"

AbstractTree::AbstractTree() : root("Root", nullptr), currNode(&root) {
    // the root node will have a null parent node
    // it is the root of the tree
    // set the current node pointer to the address of the root node
    // this is the node we start on
}