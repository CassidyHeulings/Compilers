#include "../Headers/Parser.hpp"
#include <iostream>

// TODO remove charlist token

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
        logger.debug(name, levelString + " " + nodeLoc.getName());
    }
    // log each child of the current node using recursion
    for (auto& child : nodeLoc.getChildren()) {
        // if the child exists, log next child on next tree level
        if (child) printTree(*child, treeLevel+1);
    }
}

// match the token name
void Parser::match(std::vector<std::string> expected) {
    bool isMatch = false;
    int i = 0;
    while (i < expected.size() && !isMatch) {
        // if the token we are on is the expected token or token value
        if (tokens->at(tokenIndex) == expected[i]) {
            currTree->addChild("[ " + tokenVals->at(tokenIndex) + " ]");
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
        logger.debug(name, "Error in Match at " + tokenLocs->at(tokenIndex));
        for (std::string option : expected) {
            logger.debug(name, "Expected " + option);
        }
        logger.debug(name, "Found "+ tokens->at(tokenIndex));
        tokenIndex++;
        currTree->moveUpTree();
    }
}

void Parser::parseProgram() {
    logger.test(name, "Parse Program");
    currTree->addChild("< \033[36mProgram\033[0m >");
    parseBlock();
    match({"EOP"});
    currTree->moveUpTree();
}

void Parser::parseBlock() {
    currTree->addChild("< \033[36mBlock\033[0m >");
    match({"LBRACE"});
    parseStatementList();
    match({"RBRACE"});
    currTree->moveUpTree();
}

void Parser::parseStatementList() {
    currTree->addChild("< \033[36mStatementList\033[0m >");
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
    currTree->addChild("< \033[36mStatement\033[0m >");
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
    currTree->addChild("< \033[36mPrintStatement\033[0m >");
    match({"PRINT"});
    match({"LPAREN"});
    parseExpr();
    match({"RPAREN"});
    currTree->moveUpTree();
}

void Parser::parseAssignmentStatement() {
    currTree->addChild("< \033[36mAssignmentStatement\033[0m >");
    parseId();
    match({"ASSIGN"});
    parseExpr();
    currTree->moveUpTree();
}

void Parser::parseVarDecl() {
    currTree->addChild("< \033[36mVarDecl\033[0m >");
    parseType();
    parseId();
    currTree->moveUpTree();
}

void Parser::parseWhileStatement() {
    currTree->addChild("< \033[36mWhileStatement\033[0m >");
    match({"WHILE"});
    parseBooleanExpr();
    parseBlock();
    currTree->moveUpTree();
}

void Parser::parseIfStatement() {
    currTree->addChild("< \033[36mIfStatement\033[0m >");
    match({"IF"});
    parseBooleanExpr();
    parseBlock();
    currTree->moveUpTree();
}

void Parser::parseExpr() {
    currTree->addChild("< \033[36mParseExpr\033[0m >");
    if (tokens->at(tokenIndex) == "DIGIT") {
        parseIntExpr();
    }
    else if (tokens->at(tokenIndex) == "QUOTMARK") {
        parseStringExpr();
    }
    else if (tokens->at(tokenIndex) == "LPAREN") {
        parseBooleanExpr();
    }
    else if (tokens->at(tokenIndex) == "ID") {
        parseId();
    }
    // else log error?
    currTree->moveUpTree();
}

void Parser::parseIntExpr() {
    currTree->addChild("< \033[36mIntExpr\033[0m >");
    parseDigit();
    if (tokens->at(tokenIndex) == "INTOP") {
        parseIntop();
        parseExpr();
    }
    currTree->moveUpTree();
}

void Parser::parseStringExpr() {
    currTree->addChild("< \033[36mParseStringExpr\033[0m >");
    match({"QUOTMARK"});
    parseCharList();
    match({"QUOTMARK"});
    currTree->moveUpTree();
}

void Parser::parseBooleanExpr() {
    currTree->addChild("< \033[36mBooleanExpr\033[0m >");
    match({"LPAREN"});
    parseExpr();
    parseBoolop();
    parseExpr();
    match({"RPAREN"});
    currTree->moveUpTree();
}

void Parser::parseId() {
    currTree->addChild("< \033[36mId\033[0m >");
    match({"ID"});
    currTree->moveUpTree();
}

void Parser::parseCharList() {
    currTree->addChild("< \033[36mCharList\033[0m >");
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
    currTree->addChild("< \033[36mType\033[0m >");
    match({"TYPE"});
    currTree->moveUpTree();
}

void Parser::parseChar() {
    currTree->addChild("< \033[36mChar\033[0m >");
    match({"CHAR"});
    currTree->moveUpTree();
}

void Parser::parseSpace() {
    currTree->addChild("< \033[36mSpace\033[0m >");
    match({"SPACE"});
    currTree->moveUpTree();
}

void Parser::parseDigit() {
    currTree->addChild("< \033[36mDigit\033[0m >");
    match({"DIGIT"});
    currTree->moveUpTree();
}

void Parser::parseBoolop() {
    currTree->addChild("< \033[36mBoolop\033[0m >");
    match({"BOOLOP"});
    currTree->moveUpTree();
}

void Parser::parseBoolval() {
    currTree->addChild("< \033[36mBoolval\033[0m >");
    match({"BOOLVAL"});
    currTree->moveUpTree();
}

void Parser::parseIntop() {
    currTree->addChild("< \033[36mIntop\033[0m >");
    match({"INTOP"});
    currTree->moveUpTree();
}