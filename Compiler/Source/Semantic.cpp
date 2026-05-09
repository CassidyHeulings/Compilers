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
    buildTable(root, 0, 0);
}

// TODO add errors and warnings
void Semantic::buildTable(Node& nodeLoc, int treeLevel, int childNum) {
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
        if (table->getSymbols().contains(id[0])) return; // error
        // make an entry in the scopes hashtable using the children
        table->getSymbols().addSymbol(id[0]);
        table->getSymbols().setType(id[0], type);
    }

    else if (nodeName == "AssignmentStatement") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // first child is an id
        Node& id = *nodeLoc.getChildren()[0];
        // next child is the assign value
        Node& val = *nodeLoc.getChildren()[1];

        std::string typeIs;
        // if the id doesnt exist yet
        if (!hasSymbol(id.getName()[0], table->getCurr())) {
            // warning - no type declaration before init
            // create symbol in table
            table->getSymbols().addSymbol(id.getName()[0]);
            // assign type based on input
            if (decideType(val.getName()) == "id") {
                if (hasSymbol(val.getName()[0], table->getCurr())) {
                    // set this id to used
                    getScope(val.getName()[0], table->getCurr()).getSymbols().setUsed(val.getName()[0]);
                    // get the type of the id
                    typeIs = getScope(val.getName()[0], table->getCurr()).getSymbols().getType(val.getName()[0]);
                    // assign the type to the id
                    table->getSymbols().setType(id.getName()[0], typeIs);
                }
                else return; // error - using symbol that does not exist
            }
            else {
                table->getSymbols().setType(id.getName()[0], decideType(val.getName()));
            }
        }
        // if it does exist
        else {
            // value is an id
            if (decideType(val.getName()) == "id") { 
                // value is in the symbol table
                if (hasSymbol(val.getName()[0], table->getCurr())) {
                    // set this id to used
                    logger.test(name, "setting to used");
                    getScope(val.getName()[0], table->getCurr()).getSymbols().setUsed(val.getName()[0]);
                    // see if the two types match
                    logger.test(name, "do the types match");
                    typeIs = compareType(id, val);
                    // if the types do not match, error
                    if (typeIs == "none") return; // error - not matching types
                }
                    else // value is not in symbol table
                        return; // error - val id does not exists in symbol table
            }
            else {
                logger.test(name, "getting scope");
                // if types dont match
                if (getScope(id.getName()[0], table->getCurr()).getSymbols().getType(id.getName()[0]) != decideType(val.getName()))
                    return; // error - types do not match
            }
        logger.test(name, "setting value to init");
        // set id to initialized
        table->getSymbols().setInit(id.getName()[0]);
        }
    }

    else if (nodeName == "Block") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // create a child in table
        table->addChild("Scope" + std::to_string(treeLevel) + std::string(1, 'a' + childNum)); // Scope + 0,1,... + a,b,...
        table->addSymbolsToNode();
        // increase tree level
        treeLevel++;
        // this node will be a parent
        parentNode = true;
    }

    else if (nodeName == "IfStatement" || nodeName == "WhileStatement") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // first child is first val
        Node& val1 = *nodeLoc.getChildren()[0];
        // third child is second val
        Node& val2 = *nodeLoc.getChildren()[2];

        // if both vals are ids
        if (decideType(val1.getName()) == "id" && decideType(val2.getName()) == "id") {
            logger.test(name, "both are ids");
            // check if they are in the symbol table
            logger.test(name, "checking if they are in symbol table");
            if (!hasSymbol(val1.getName()[0], table->getCurr()))
                return; // error - symbol not in symbol table
            if (!hasSymbol(val2.getName()[0], table->getCurr()))
                return; // error - symbol not in symbol table
            // mark second id as used
            logger.test(name, "second id used");
            getScope(val2.getName()[0], table->getCurr()).getSymbols().setUsed(val2.getName()[0]);
            // compare the types
            logger.test(name, "comparing types");
            std::string typeIs = compareType(val1, val2);
            if (typeIs == "none") return; // error - types do not match
        }
        // first val is id
        else if (decideType(val1.getName()) == "id") {
            // check if in the symbol table
            if (!hasSymbol(val1.getName()[0], table->getCurr()))
                return; // error - symbol not in symbol table
            // mark val as used
            getScope(val1.getName()[0], table->getCurr()).getSymbols().setUsed(val1.getName()[0]);
            // compare the types
            if (getScope(val1.getName()[0], table->getCurr()).getSymbols().getType(val1.getName()[0]) != decideType(val2.getName()))
                return; // error - mismatch types
        }
    }

    else if (nodeName == "PrintStatement") {
        logger.test(name, nodeName + " " + std::to_string(treeLevel));
        // first child is value
        std::string val = nodeLoc.getChildren()[0]->getName();
        // check if value is an id
        if (decideType(val) == "id") {
            // check if val is in table
            if (!hasSymbol(val[0], table->getCurr()))
                return; // error - symbol not in symbol table
            logger.test(name, "marking print val as used");
            // if it is, mark as used
            getScope(val[0], table->getCurr()).getSymbols().setUsed(val[0]);
        }
    }

    // for each child of the node
    for (auto& child : nodeLoc.getChildren()) {
        // recursively keep building the ast
        if (child) {
            buildTable(*child, treeLevel, childNum);
        }
        if (child->getName() == "Block") childNum++;
    }

    // if a parent node was created, move up the tree to the level above
    if (parentNode) table->moveUpTree();
}

void Semantic::createPrintTable(Node& nodeLoc, int row, int column) {
    // write block into table
    std::vector<std::string> block = nodeLoc.getBlock();
    for (int r = 0; r < block.size(); r++) {
        // extend the table to how this row
        expandRows(row + r);
        // extend the table row to have enough columns
        expandColumns(row + r, column + block[r].size());
        // write each char into the table from the line
        for (int c = 0; c < block[r].size(); c++) {
            fullTable[row + r][column + c] = block[r][c];
        }
    }
    // children of this node will be below this node
    int childRow = row + nodeLoc.getBlock().size() + 1;
    // children of this node will start before this node in columns
    int startColumn = column;
    if (nodeLoc.getChildren().size() > 1) {
        startColumn = column - 36 / 2;
    }

    // keeping track of number of children
    int i = 0;
    // log each child of the current node using recursion
    for (auto& child : nodeLoc.getChildren()) {
        // if the child exists, log next child on next tree level
        if (child) {
            // make the child column based on what number child it is
            int childColumn = startColumn + i * 36;
            createPrintTable(*child, childRow, childColumn);
            // increase i for next child
            i++;
        }
    }
}

void Semantic::printTable(Node& nodeLoc) {
    // reset the table
    fullTable.empty();
    // start creating the table with first block at (0, size of block with spacing)
    createPrintTable(nodeLoc, 0, 20);
    logger.debug(name, "Symbol Table:");
    // print the symbol table
    for (std::string line : fullTable)
        logger.debug(name, line);
}

void Semantic::expandRows(int row) {
    while (fullTable.size() <= row)
        fullTable.push_back("");
}

void Semantic::expandColumns(int row, int column) {
    if (fullTable.at(row).size() < column)
        fullTable.at(row).resize(column, ' ');
}

std::string Semantic::decideType(std::string val) {
    logger.test(name, "Checking type of " + val);
    if (val.empty())
        return "none";
    // string
    if (val[0] == '"')
        return "string";
    // int
    else if (std::isdigit(val[0]))
        return "int";
    // boolean
    else if (val == "true" || val == "false")
        return "boolean";
    // id, not a type
    else
        return "id";
}

bool Semantic::hasSymbol(char id, Node& currScope) {
    logger.test(name, "Checking scope" + currScope.getName() + " Checking id " + id);
    // the symbol is in this nodes symbol table
    if (currScope.getSymbols().contains(id)) return true;
    // check the parent scope
    if (currScope.getParent() != nullptr) return hasSymbol(id, *currScope.getParent());
    // symbol doesnt exist in any parent scopes
    return false;
}

// only run if hasSymbol is true to guarenteed address return
Node& Semantic::getScope(char id, Node& currScope) {
    logger.test(name, "Checking scope" + currScope.getName());
    // the symbol is in this nodes symbol table
    if (currScope.getSymbols().contains(id)) return currScope;
    // check the parent scope
    if (currScope.getParent() != nullptr) return getScope(id, *currScope.getParent());
}

// checks if in symbol tree, does not create new entries in symbol table
std::string Semantic::compareType(Node& val1Loc, Node& val2Loc) {
    // the value types
    std::string val1Type = "none";
    std::string val2Type = "none";

    // value 1
    // if node has children, get the type from the children (middle child is some operation)
    if (val1Loc.getChildren().size() > 0) val1Type = compareType(*val1Loc.getChildren()[0], *val1Loc.getChildren()[2]);
    // value is an id
    else if (decideType(val1Loc.getName()) == "id") {
        logger.test(name, "getting val1 scope");
        // get the scope of the value
        Node& val1Scope = getScope(val1Loc.getName()[0], table->getCurr());
        logger.test(name, "getting val1 type");
        // get the type
        val1Type = val1Scope.getSymbols().getType(val1Loc.getName()[0]);
    }
    // value is not an id
    else {
        // set the type of the value
        val1Type = decideType(val1Loc.getName());
    }

    // value 2
    // if node has children, get the type from the children (middle child is some operation)
    if (val2Loc.getChildren().size() > 0) val2Type = compareType(*val2Loc.getChildren()[0], *val2Loc.getChildren()[2]);
    // value is an id
    else if (decideType(val2Loc.getName()) == "id") {
        // get the scope of the value
        Node& val2Scope = getScope(val2Loc.getName()[0], table->getCurr());
        // mark as used
        val2Scope.getSymbols().setUsed(val2Loc.getName()[0]);
        // get the type
        val2Type = val2Scope.getSymbols().getType(val2Loc.getName()[0]);
    }
    // value is not an id
    else {
        // set the type of the value
        val2Type = decideType(val2Loc.getName());
    }

    // same type
    if (val1Type == val2Type) return val1Type;
    // different type
    else return "none";
}