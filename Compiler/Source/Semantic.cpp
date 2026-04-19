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
    // get the token name
    std::string nodeName = nodeLoc.getName();
    logger.test(name, nodeName);
    // check if the node is one of the tokens we want
    if (nodeName == "< \033[36mBlock\033[0m >" 
        || nodeName == "< \033[36VarDecl\033[0 >" 
        || nodeName == "< \033[36AssignmentStatement\033[0 >" 
        || nodeName == "< \033[36PrintStatement\033[0 >")
        // create a child in ast of the token
        currTree->addChild(nodeName);
    // we want the child of these tokens (their value)
    else if (nodeName == "< Type >" || nodeName == "< Id >" || nodeName == "< Digit >")
        currTree->addChild(nodeLoc.getChildren()[0]->getName()); // each will only have one child
    for (auto& child : nodeLoc.getChildren()) {
        // move to children
        if (child) buildAst(*child);
    }
}

void Semantic::printTree(Node& nodeLoc, int treeLevel) {
    // for logging depth in tree
    std::string levelString = "|";
    for (int i = 0; i < treeLevel; i++) {
        levelString += "--";
    }
    // log the node using the node name (ignore the root)
    if (nodeLoc.getName() != "Root") {
        logger.debug(name, levelString + " " + nodeLoc.getName());
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