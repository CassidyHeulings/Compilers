#include "../Headers/Parser.hpp"
#include <iostream>

Parser::Parser(Logger& loggerInstance, const std::string& processName) : logger(loggerInstance) {
    name = processName;
    logger.test(name, "hi im the parser");
}

void Parser::startParse() {
    // add a new tree to the list of trees
    allTrees.push_back(std::make_unique<ParseTree>());
    // set the current tree pointer to the new tree added to vector
    currTree = allTrees.back().get();
    parseProgram();
}

void Parser::match(const std::string& expected) {
    // x = checkExpected
    // if x addNode(leaf, x)
    // else error
}

void Parser::parseProgram() {
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
    // if value in { parseStatement() } {
        // parseStatement();
        // parseStatementList();
        // }
    // else
        // epsilon production
    currTree->moveUpTree();
}

void Parser::parseStatement() {
    currTree->addChild("Statement");
    // switch (value)
    // print
    // assignment
    // vardecl
    // while
    // if
    // block
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
    currTree->addChild("ParseExpr");
    // switch value
    // int expr
    // str expr
    // bool expr
    // id
    currTree->moveUpTree();
}

void Parser::parseIntExpr() {
    currTree->addChild("IntExpr");
    parseDigit();
    // if value in {intop}
        //parseIntop();
        //parseExpr();
    currTree->moveUpTree();
}

void Parser::parseStringExpr() {
    currTree->addChild("ParseStringExpr");
    match("QUOTMARK");
    parseCharList();
    match("QUOTMARK");
    currTree->moveUpTree();
}

void Parser::parseBooleanExpr() {
    currTree->addChild("BooleanExpr");
    match("LPAREN");
    parseExpr();
    parseBoolop();
    parseExpr();
    match("RPAREN");
    currTree->moveUpTree();
}

void Parser::parseId() {
    currTree->addChild("Id");
    match("ID");
    currTree->moveUpTree();
}

void Parser::parseCharList() {
    currTree->addChild("CharList");
    // if value in char
        // parseChar();
        // parseCharList();
    // else if value in space
        // parseSpace();
    // else
        // epsilon production 
}

bool Parser::parseType() {

}

bool Parser::parseChar() {

}

bool Parser::parseSpace() {

}

bool Parser::parseDigit() {

}

bool Parser::parseBoolop() {

}

bool Parser::parseBoolval() {

}

bool Parser::parseIntop() {

}