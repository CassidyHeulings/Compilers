#include "../Headers/Semantic.hpp"

Semantic::Semantic(Logger& loggerInstance, const std::string& processName) 
    : logger(loggerInstance) {
    name = processName;
    logger.test(name, "hi im semantic analysis");
}

/* void Semantic::setValues(std::vector<std::string>& newTokens, std::vector<std::string>& newVals) {
    tokens = &newTokens;
    tokenVals = &newVals;
} */

void Semantic::createAst(std::__1::unique_ptr<ParseTree>& cst) {
    // add a new ast to the list of trees
    allTrees.push_back(std::make_unique<AbstractTree>());
    // set the current tree pointer to the new tree added to vector
    currTree = allTrees.back().get();
    // retrieve the root node of the tree
    Node& root = cst->retrieveRoot();
    // start building the ast
    buildAst(root);
}

void Semantic::buildAst(Node& nodeLoc) {
    // track whether a parent node was created
    bool parentNode = false;
    // get the token name
    std::string nodeName = nodeLoc.getName();
    logger.test(name, nodeName);

    // check if the node is one of the tokens we want
    if (nodeName == "Block" 
        || nodeName == "VarDecl" 
        || nodeName == "AssignmentStatement" 
        || nodeName == "PrintStatement"
        || nodeName == "IfStatement"
        || nodeName == "WhileStatement") {
        // create a child in ast of the token
        currTree->addChild(nodeName);
        // we will be moving into these nodes to create children
        // will need to come back up from these nodes to get back to the level above
        // we dont want to move up to the level above until after all the children are taken care of
        parentNode = true;
    }
    // we want the child of these tokens (their value)
    else if (nodeName == "Type" 
        || nodeName == "Id" 
        || nodeName == "Digit" 
        || nodeName == "Boolval"
        || nodeName == "Boolop"
        || nodeName == "Intop") {
        currTree->addChild(nodeLoc.getChildren()[0]->getName()); // each will only have one child
        // move up the tree immediately
        currTree->moveUpTree();
        // we do not set astNode to true - these nodes will have no children
        // we want to move up the tree immediatly since they will never have their own children
    }
    // check for strings (charlists)
    else if (nodeName == "\"") {
        // add quotes
        currString += nodeName;
        // if it is the end of the string
        if (currString.size() > 1) {
            // add the string as an ast node
            currTree->addChild(currString);
            // move back up the tree -> this node will have no children
            currTree->moveUpTree();
            // make string empty for next string
            currString = "";
        }
    }
    else if (nodeName == "Char" || nodeName == "Space") {
        // add the next char of the charlist to the string
        currString += nodeLoc.getChildren()[0]->getName();
    }

    // for each child of the node
    for (auto& child : nodeLoc.getChildren()) {
        // recursively keep building the ast
        if (child) buildAst(*child);
    }

    // if a parent node was created, move up the tree to the level above
    if (parentNode) currTree->moveUpTree();
}

void Semantic::addToString(char letter) {
    currString += letter;
}

void Semantic::resetString() {
    currString = "";
}

void Semantic::printTree(Node& nodeLoc, int treeLevel) {
    // for logging depth in tree
    std::string levelString = "|";
    for (int i = 0; i < treeLevel; i++) {
        levelString += "--";
    }
    // log the node using the node name (ignore the root)
    if (nodeLoc.getName() != "Root") {
        logger.debug(name, levelString + " " + "\033[36m" + nodeLoc.getName() + "\033[0m");
    }
    // log each child of the current node using recursion
    for (auto& child : nodeLoc.getChildren()) {
        // if the child exists, log next child on next tree level
        if (child) printTree(*child, treeLevel+1);
    }
}

std::vector<std::unique_ptr<AbstractTree>>& Semantic::getTrees() {
    return allTrees;
}