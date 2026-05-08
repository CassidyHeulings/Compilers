#include "../Headers/Semantic.hpp"

Semantic::Semantic(Logger& loggerInstance, const std::string& processName) 
    : logger(loggerInstance) {
    name = processName;
    logger.test(name, "hi im semantic analysis");
}

void Semantic::createAst(std::__1::unique_ptr<Tree>& cst) {
    // create a pointer to the tree
    tree = std::make_unique<Tree>();
    // make sure string starts empty in between programs
    currString.clear();
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
        tree->addChild(nodeName);
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
        tree->addChild(nodeLoc.getChildren()[0]->getName()); // each will only have one child
        // move up the tree immediately
        tree->moveUpTree();
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
            tree->addChild(currString);
            // move back up the tree -> this node will have no children
            tree->moveUpTree();
            // make string empty for next string
            currString.clear();
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
    if (parentNode) tree->moveUpTree();
}

void Semantic::printTree(Node& nodeLoc, int treeLevel) {
    // for logging depth in tree
    std::string levelString = "|";
    for (int i = 0; i < treeLevel; i++) {
        levelString += "--";
    }
    // log the node using the node name (ignore the root)
    if (nodeLoc.getName() != "Root") {
        // printing things from the input code
        if (nodeLoc.getName().size() == 1 || nodeLoc.getName()[0] == '\"')
            logger.debug(name, levelString + " " + "[ " + nodeLoc.getName() + " ]");
        // printing token names
        else
            logger.debug(name, levelString + " " + "< \033[36m" + nodeLoc.getName() + "\033[0m >");
    }
    // log each child of the current node using recursion
    for (auto& child : nodeLoc.getChildren()) {
        // if the child exists, log next child on next tree level
        if (child) printTree(*child, treeLevel+1);
    }
}

std::unique_ptr<Tree>& Semantic::getTree() {
    return tree;
}

std::unique_ptr<Tree>& Semantic::getTable() {
    return table;
}

void Semantic::createTable(std::__1::unique_ptr<Tree>& ast) {
    // create a pointer to the tree
    table = std::make_unique<Tree>();
    // retrieve the root node of the tree
    Node& root = ast->retrieveRoot();
    // start building the ast
    buildTable(root, -1);
}

void Semantic::buildTable(Node& nodeLoc, int treeLevel) {
    // track whether a parent node was created
    bool parentNode = false;
    // get the token name
    std::string nodeName = nodeLoc.getName();

    // check if the node is related to var initialization
    if (nodeName == "VarDecl") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // first child is type
        std::string type = nodeLoc.getChildren()[0]->getName();
        // second child is variable
        std::string id = nodeLoc.getChildren()[1]->getName();
        // check if the var alr exists
        // make an entry in the scopes hashtable using the children
    }
    else if (nodeName == "AssignmentStatement") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // first child is variable
        std::string id = nodeLoc.getChildren()[0]->getName();
        // check if var alr exists
        // make an entry into hash table
    }
    else if (nodeName == "Block") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // create a child in table
        table->addChild(nodeName + std::to_string(treeLevel)); // TODO make 1a not 1 using ascii
        // this node will be a parent
        parentNode = true;
    }
    else if (nodeName == "IfStatement") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // create a child in table
        table->addChild(nodeName + std::to_string(treeLevel)); // TODO make 1a not 1 using ascii
        // this node will be a parent
        parentNode = true;
        // mark child var as used
        // mark as parent
    }
    else if (nodeName == "WhileStatement") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // create a child in table
        table->addChild(nodeName + std::to_string(treeLevel)); // TODO make 1a not 1 using ascii
        // this node will be a parent
        parentNode = true;
        // mark child var as used
    }
    else if (nodeName == "PrintStatement") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // mark child var as used
        // move up the tree immediately
    }

    // for each child of the node
    for (auto& child : nodeLoc.getChildren()) {
        // recursively keep building the ast
        if (child) buildTable(*child, treeLevel+1);
    }

    // if a parent node was created, move up the tree to the level above
    if (parentNode) table->moveUpTree();
}

void Semantic::printTable(Node& nodeLoc, int treeLevel) {
    // for logging depth in tree
    std::string levelString = "|";
    for (int i = 0; i < treeLevel; i++) {
        levelString += "--";
    }
    // log the node using the node name (ignore the root)
    if (nodeLoc.getName() != "Root") {
        // printing things from the input code
        if (nodeLoc.getName().size() == 1 || nodeLoc.getName()[0] == '\"')
            logger.debug(name, levelString + " " + "[ " + nodeLoc.getName() + " ]");
        // printing token names
        else
            logger.debug(name, levelString + " " + "< \033[36m" + nodeLoc.getName() + "\033[0m >");
    }
    // log each child of the current node using recursion
    for (auto& child : nodeLoc.getChildren()) {
        // if the child exists, log next child on next tree level
        if (child) printTable(*child, treeLevel+1);
    }
}