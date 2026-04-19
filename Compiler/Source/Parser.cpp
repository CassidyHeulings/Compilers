#include "../Headers/Parser.hpp"
#include <iostream>

Parser::Parser(Logger& loggerInstance, const std::string& processName) 
    : logger(loggerInstance), tokens(nullptr), tokenVals(nullptr), tokenLocs(nullptr) {
    name = processName;
    logger.test(name, "hi im the parser");
}

void Parser::setValues(std::vector<std::string>& newTokens, std::vector<std::string>& newVals, std::vector<std::string>& newLocs) {
    tokens = &newTokens;
    tokenVals = &newVals;
    tokenLocs = &newLocs;
}

// TODO double check
void Parser::checkForNewTree() {
    // if there are still tokens after we are back at root node
    if (tokenIndex < tokens->size()) {
        startParse();
    }
}

void Parser::startParse() {
    // add a new tree to the list of trees
    allTrees.push_back(std::make_unique<ParseTree>());
    // set the current tree pointer to the new tree added to vector
    currTree = allTrees.back().get();
    parseProgram();
    checkForNewTree();
}

std::vector<std::unique_ptr<ParseTree>>& Parser::getTrees() {
    return allTrees;
}

void Parser::printTree(Node& nodeLoc, int treeLevel) {
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

// match the token name
void Parser::match(std::string expected) {
    bool isMatch = false;
    int i = 0;
    while (i < expected.size() && !isMatch) {
        // if the token we are on is the expected token or token value
        if (tokens->at(tokenIndex) == expected) {
            currTree->addChild(tokenVals->at(tokenIndex));
            // move to next token
            tokenIndex++;
            // found the match
            isMatch = true;
            // move up to parent, match will never have a child
            currTree->moveUpTree();
        }
        i++;
    }
    if (!isMatch) {
        // TODO error handling using logger error
        logger.error(name, 2, "Expected " + getTokenValues(expected) + " found " + tokenVals->at(tokenIndex) + " at " + tokenLocs->at(tokenIndex));
        // end the program if debug is not on
        if (!logger.getDebug()) {
            logger.endProgram();
            std::exit(0);
        }
        // continue after error
        else {
            tokenIndex++;
            if (currTree->moveUpTree()) currTree->moveUpTree();
        }
    }
}

void Parser::parseProgram() {
    logger.test(name, "Parse Program");
    currTree->addChild("Program");
    parseBlock();
    match("EOP");
    currTree->moveUpTree();
}

void Parser::parseBlock() {
    currTree->addChild("Block");
    match("LBRACE");
    parseStatementList();
    match("RBRACE");
    currTree->moveUpTree();
}

void Parser::parseStatementList() {
    currTree->addChild("StatementList");
    if (tokens->at(tokenIndex) == "PRINT" ||
        tokens->at(tokenIndex) == "ID" ||
        tokens->at(tokenIndex) == "TYPE" ||
        tokens->at(tokenIndex) == "WHILE" ||
        tokens->at(tokenIndex) == "IF" ||
        tokens->at(tokenIndex) == "LBRACE") {
        parseStatement();
        parseStatementList();
    }
    else {
        logger.test(name, "Epsilon in statement list");
    }
    currTree->moveUpTree();
}

void Parser::parseStatement() {
    currTree->addChild("Statement");
    if (tokens->at(tokenIndex) == "PRINT") {
        parsePrintStatement();
    }
    else if (tokens->at(tokenIndex) == "ID") {
        parseAssignmentStatement();
    }
    else if (tokens->at(tokenIndex) == "TYPE") {
        parseVarDecl();
    }
    else if (tokens->at(tokenIndex) == "WHILE") {
        parseWhileStatement();
    }
    else if (tokens->at(tokenIndex) == "IF") {
        parseIfStatement();
    }
    else if (tokens->at(tokenIndex) == "LBRACE") {
        parseBlock();
    }
    // else error??
    currTree->moveUpTree();
}

void Parser::parsePrintStatement() {
    currTree->addChild("PrintStatement");
    match("PRINT");
    match("LPAREN");
    parseExpr();
    match("RPAREN");
    currTree->moveUpTree();
}

void Parser::parseAssignmentStatement() {
    currTree->addChild("AssignmentStatement");
    parseId();
    match("ASSIGN");
    parseExpr();
    currTree->moveUpTree();
}

void Parser::parseVarDecl() {
    currTree->addChild("VarDecl");
    parseType();
    parseId();
    currTree->moveUpTree();
}

void Parser::parseWhileStatement() {
    currTree->addChild("WhileStatement");
    match("WHILE");
    parseBooleanExpr();
    parseBlock();
    currTree->moveUpTree();
}

void Parser::parseIfStatement() {
    currTree->addChild("IfStatement");
    match("IF");
    parseBooleanExpr();
    parseBlock();
    currTree->moveUpTree();
}

void Parser::parseExpr() {
    currTree->addChild("Expr");
    if (tokens->at(tokenIndex) == "DIGIT") {
        parseIntExpr();
    }
    else if (tokens->at(tokenIndex) == "QUOTMARK") {
        parseStringExpr();
    }
    else if (tokens->at(tokenIndex) == "LPAREN" || tokens->at(tokenIndex) == "BOOLVAL") {
        parseBooleanExpr();
    }
    else if (tokens->at(tokenIndex) == "ID") {
        parseId();
    }
    // else log error?
    currTree->moveUpTree();
}

void Parser::parseIntExpr() {
    currTree->addChild("IntExpr");
    parseDigit();
    if (tokens->at(tokenIndex) == "INTOP") {
        parseIntop();
        parseExpr();
    }
    currTree->moveUpTree();
}

void Parser::parseStringExpr() {
    currTree->addChild("StringExpr");
    match("QUOTMARK");
    parseCharList();
    match("QUOTMARK");
    currTree->moveUpTree();
}

void Parser::parseBooleanExpr() {
    currTree->addChild("BooleanExpr");
    if (tokens->at(tokenIndex) == "LPAREN") {
        match("LPAREN");
        parseExpr();
        parseBoolop();
        parseExpr();
        match("RPAREN");
    }
    else if (tokens->at(tokenIndex) == "BOOLVAL") {
        match("BOOLVAL");
    }
    currTree->moveUpTree();
}

void Parser::parseId() {
    currTree->addChild("Id");
    match("ID");
    currTree->moveUpTree();
}

void Parser::parseCharList() {
    currTree->addChild("CharList");
    // all chars are under token ID
    if (tokens->at(tokenIndex) == "CHAR") {
        parseChar();
        parseCharList();
    }
    else if (tokenVals->at(tokenIndex) == " ") {
        parseSpace();
        parseCharList();
    }
    else {
        logger.test(name, "Epsilon in Charlist");
    }
    currTree->moveUpTree();
}

void Parser::parseType() {
    currTree->addChild("Type");
    match("TYPE");
    currTree->moveUpTree();
}

void Parser::parseChar() {
    currTree->addChild("Char");
    match("CHAR");
    currTree->moveUpTree();
}

// TODO keep space child?
void Parser::parseSpace() {
    currTree->addChild("Space");
    match("SPACE");
    currTree->moveUpTree();
}

void Parser::parseDigit() {
    currTree->addChild("Digit");
    match("DIGIT");
    currTree->moveUpTree();
}

void Parser::parseBoolop() {
    currTree->addChild("Boolop");
    match("BOOLOP");
    currTree->moveUpTree();
}

void Parser::parseBoolval() {
    currTree->addChild("Boolval");
    match("BOOLVAL");
    currTree->moveUpTree();
}

void Parser::parseIntop() {
    currTree->addChild("Intop");
    match("INTOP");
    currTree->moveUpTree();
}

std::string Parser::getTokenValues(std::string token) {
    if (token == "LBRACE") return "{";
    else if (token == "RBRACE") return "}";
    else if (token == "ID") return "a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p, q, r, s, t, u, v, w, x, y, z";
    else if (token == "IF") return "if";
    else if (token == "WHILE") return "while";
    else if (token == "PRINT") return "print";
    else if (token == "BOOLVAL") return "true, false";
    else if (token == "TYPE") return "int, string, boolean";
    else if (token == "DIGIT") return "0, 1, 2, 3, 4, 5, 6, 7, 8, 9";
    else if (token == "ASSIGN") return "=";
    else if (token == "BOOLOP") return "==, !=";
    else if (token == "INTOP") return "+";
    else if (token == "LPAREN") return "(";
    else if (token == "RPAREN") return ")";
    else if (token == "QUOTMARK" || token == "SPACE" || token == "CHAR") return "lowercase string of letters";
    else if (token == "EOP") return "$";
    else return "something else";
}