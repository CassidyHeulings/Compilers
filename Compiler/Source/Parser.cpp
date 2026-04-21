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
    tokenIndex = 0;
}

void Parser::startParse() {
    // create a pointer to the tree
    tree = std::make_unique<Tree>();
    // start the parse
    parseProgram();
}

std::unique_ptr<Tree>& Parser::getTree() {
    return tree;
}

void Parser::printTree(Node& nodeLoc, int treeLevel) {
    // for logging depth in tree
    std::string levelString = "|";
    for (int i = 0; i < treeLevel; i++) {
        levelString += "--";
    }
    // log the node using the node name (ignore the root)
    if (nodeLoc.getName() != "Root") {
        // printing things from the input code
        if (nodeLoc.getName().size() == 1)
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

// match the token name
void Parser::match(std::string expected) {
    logger.test(name, "Testing match");
    // if the token we are on is the expected token or token value
    if (tokens->at(tokenIndex) == expected) {
        tree->addChild(tokenVals->at(tokenIndex));
        // move to next token
        logger.test(name, "next token");
        tokenIndex++;
        // move up to parent, match will never have a child
        tree->moveUpTree();
    }
    else {
        logger.error(name, 2, "Expected " + getTokenValues(expected) + " found " + tokenVals->at(tokenIndex) + " at " + tokenLocs->at(tokenIndex));
        logger.warning(name, 3, "Fix the error to print the full tree.");
        // end the program parsing
        return;
    }
}

void Parser::parseProgram() {
    logger.test(name, "Parse Program");
    tree->addChild("Program");
    parseBlock();
    match("EOP");
    tree->moveUpTree();
}

void Parser::parseBlock() {
    tree->addChild("Block");
    match("LBRACE");
    parseStatementList();
    match("RBRACE");
    tree->moveUpTree();
}

void Parser::parseStatementList() {
    tree->addChild("StatementList");
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
    tree->moveUpTree();
}

void Parser::parseStatement() {
    tree->addChild("Statement");
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
    tree->moveUpTree();
}

void Parser::parsePrintStatement() {
    tree->addChild("PrintStatement");
    match("PRINT");
    match("LPAREN");
    parseExpr();
    match("RPAREN");
    tree->moveUpTree();
}

void Parser::parseAssignmentStatement() {
    tree->addChild("AssignmentStatement");
    parseId();
    match("ASSIGN");
    parseExpr();
    tree->moveUpTree();
}

void Parser::parseVarDecl() {
    tree->addChild("VarDecl");
    parseType();
    parseId();
    tree->moveUpTree();
}

void Parser::parseWhileStatement() {
    tree->addChild("WhileStatement");
    match("WHILE");
    parseBooleanExpr();
    parseBlock();
    tree->moveUpTree();
}

void Parser::parseIfStatement() {
    tree->addChild("IfStatement");
    match("IF");
    parseBooleanExpr();
    parseBlock();
    tree->moveUpTree();
}

void Parser::parseExpr() {
    tree->addChild("Expr");
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
    tree->moveUpTree();
}

void Parser::parseIntExpr() {
    tree->addChild("IntExpr");
    parseDigit();
    if (tokens->at(tokenIndex) == "INTOP") {
        parseIntop();
        parseExpr();
    }
    tree->moveUpTree();
}

void Parser::parseStringExpr() {
    tree->addChild("StringExpr");
    match("QUOTMARK");
    parseCharList();
    match("QUOTMARK");
    tree->moveUpTree();
}

void Parser::parseBooleanExpr() {
    tree->addChild("BooleanExpr");
    if (tokens->at(tokenIndex) == "LPAREN") {
        match("LPAREN");
        parseExpr();
        parseBoolop();
        parseExpr();
        match("RPAREN");
    }
    else if (tokens->at(tokenIndex) == "BOOLVAL") {
        parseBoolval();
    }
    tree->moveUpTree();
}

void Parser::parseId() {
    tree->addChild("Id");
    match("ID");
    tree->moveUpTree();
}

void Parser::parseCharList() {
    tree->addChild("CharList");
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
    tree->moveUpTree();
}

void Parser::parseType() {
    tree->addChild("Type");
    match("TYPE");
    tree->moveUpTree();
}

void Parser::parseChar() {
    tree->addChild("Char");
    match("CHAR");
    tree->moveUpTree();
}

// TODO keep space child?
void Parser::parseSpace() {
    tree->addChild("Space");
    match("SPACE");
    tree->moveUpTree();
}

void Parser::parseDigit() {
    tree->addChild("Digit");
    match("DIGIT");
    tree->moveUpTree();
}

void Parser::parseBoolop() {
    tree->addChild("Boolop");
    match("BOOLOP");
    tree->moveUpTree();
}

void Parser::parseBoolval() {
    tree->addChild("Boolval");
    match("BOOLVAL");
    tree->moveUpTree();
}

void Parser::parseIntop() {
    tree->addChild("Intop");
    match("INTOP");
    tree->moveUpTree();
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