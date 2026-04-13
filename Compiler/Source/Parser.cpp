#include "../Headers/Parser.hpp"
#include <iostream>

// TODO remove charlist token

Parser::Parser(Logger& loggerInstance, const std::string& processName) : logger(loggerInstance), tokens(nullptr), tokenVals(nullptr) {
    name = processName;
    logger.test(name, "hi im the parser");
}

void Parser::setValues(std::vector<std::string>& newTokens, std::vector<std::string>& newVals) {
    tokens = &newTokens;
    tokenVals = &newVals;
}

// TODO double check
void Parser::checkForNewTree() {
    // if there are still tokens after we are back at root node
    if (tokens->size() > 0) {
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

// match the token name
void Parser::match(std::vector<std::string> expected) {
    bool isMatch = false;
    int i = 0;
    while (i < expected.size() && !isMatch) {
        // if the token we are on is the expected token or token value
        if (tokens->at(tokenIndex) == expected[i]) {
            currTree->addChild(tokenVals->at(tokenIndex));
            // move to next token
            tokenIndex++;
            // found the match
            isMatch = true;
            // move up to parent, match will never have a child
            currTree->moveUpTree();
        }
    }
    if (!isMatch) {
        // log error

    }
}

// match the token value for tokens that have multiple values
void Parser::matchValue(std::vector<std::string> expected) {
    // while loop control values
    bool isMatch = false;
    int i = 0;
    while (i < expected.size() && !isMatch) {
        // if the token we are on is the expected token or token value
        if (tokenVals->at(tokenIndex) == expected[i]) {
            // dont add a child or change index: match() will do this
            // found the match
            isMatch = true;
        }
    }
    if (!isMatch) {
        // log error
        
    }
}

void Parser::parseProgram() {
    currTree->addChild("Program");
    parseBlock();
    match({"EOP"});
    currTree->moveUpTree();
}

void Parser::parseBlock() {
    currTree->addChild("Block");
    match({"LBRACE"});
    parseStatementList();
    match({"RBRACE"});
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
        // epsilon production
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
    match({"PRINT"});
    match({"LPAREN"});
    parseExpr();
    match({"RPAREN"});
    currTree->moveUpTree();
}

void Parser::parseAssignmentStatement() {
    currTree->addChild("AssignmentStatement");
    parseId();
    match({"ASSIGN"});
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
    match({"WHILE"});
    parseBooleanExpr();
    parseBlock();
    currTree->moveUpTree();
}

void Parser::parseIfStatement() {
    currTree->addChild("IfStatement");
    match({"IF"});
    parseBooleanExpr();
    parseBlock();
    currTree->moveUpTree();
}

void Parser::parseExpr() {
    currTree->addChild("ParseExpr");
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
    currTree->addChild("IntExpr");
    parseDigit();
    if (tokens->at(tokenIndex) == "INTOP") {
        parseIntop();
        parseExpr();
    }
    currTree->moveUpTree();
}

void Parser::parseStringExpr() {
    currTree->addChild("ParseStringExpr");
    match({"QUOTMARK"});
    parseCharList();
    match({"QUOTMARK"});
    currTree->moveUpTree();
}

void Parser::parseBooleanExpr() {
    currTree->addChild("BooleanExpr");
    match({"LPAREN"});
    parseExpr();
    parseBoolop();
    parseExpr();
    match({"RPAREN"});
    currTree->moveUpTree();
}

void Parser::parseId() {
    currTree->addChild(tokenVals->at(tokenIndex));
    match({"ID"});
    currTree->moveUpTree();
}

// TODO add space token
void Parser::parseCharList() {
    currTree->addChild("CharList");
    // all chars are under token ID
    if (tokens->at(tokenIndex) == "ID") {
        parseChar();
        parseCharList();
    }
    else if (tokenVals->at(tokenIndex) == " ") {
        parseSpace();
    }
    else {
        // epsilon production 
    }
}

void Parser::parseType() {
    currTree->addChild(tokenVals->at(tokenIndex));
    match({"INT", "STRING", "BOOLEAN"});
    currTree->moveUpTree();
}

// TODO check charlist token
bool Parser::parseChar() {
    currTree->addChild(tokenVals->at(tokenIndex)); // will make a child for every char rn
    match({"ID"}); // matches token ID because all characters are considered IDs in lex
    currTree->moveUpTree();
}

bool Parser::parseSpace() {
    currTree->addChild(tokenVals->at(tokenIndex));
    match({"SPACE"});
    currTree->moveUpTree();
}

bool Parser::parseDigit() {
    currTree->addChild(tokenVals->at(tokenIndex));
    match({"DIGIT"});
    currTree->moveUpTree();
}

bool Parser::parseBoolop() {
    currTree->addChild(tokenVals->at(tokenIndex));
    match({"BOOLOP"});
    currTree->moveUpTree();
}

bool Parser::parseBoolval() {
    currTree->addChild(tokenVals->at(tokenIndex));
    match({"BOOLVAL"});
    currTree->moveUpTree();
}

bool Parser::parseIntop() {
    currTree->addChild(tokenVals->at(tokenIndex));
    match({"INTOP"});
    currTree->moveUpTree();
}