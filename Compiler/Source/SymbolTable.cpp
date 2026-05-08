#include "../Headers/SymbolTable.hpp"

SymbolTable::SymbolTable(Logger& loggerInstance, const std::string& processName) 
    : logger(loggerInstance) {
    name = processName;
    logger.test(name, "hi im the symbol table");
}

std::unique_ptr<Tree>& SymbolTable::getTable() {
    return table;
}

void SymbolTable::createTable(std::__1::unique_ptr<Tree>& ast) {
    // create a pointer to the tree
    table = std::make_unique<Tree>();
    // retrieve the root node of the tree
    Node& root = ast->retrieveRoot();
    // start building the ast
    buildTable(root, -1);
}

void SymbolTable::buildTable(Node& nodeLoc, int treeLevel) {
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

void SymbolTable::printTable(Node& nodeLoc, int treeLevel) {
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