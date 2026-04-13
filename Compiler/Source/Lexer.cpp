#include "../Headers/Lexer.hpp"

Lexer::Lexer(Logger& loggerInstance, const std::string& processName) : logger(loggerInstance) {
    name = processName;
    logger.test(name, "hi im the lexer");
    isQuotes = false;
    buffer = ""; // set buffer to empty
    maxBuffer = INT_MAX;
    currState = 1; // start at first state
}

void Lexer::clearBuffer() {
    buffer = ""; // set buffer to empty
}

void Lexer::resetState() {
    currState = 1; // reset the state
}

void Lexer::resetMaxBuffer() {
    maxBuffer = INT_MAX;
}

bool Lexer::getIsQuotes() {
    return isQuotes;
}

int Lexer::getBufferLength() {
    return buffer.length();
}

int Lexer::findIndex(char charVal) {
    int indexVal = -1; // index of the character
    int currIndex = 0; // index currently being checked
    
    // while the index hasn't been found and current index is within the character array
    while ((indexVal == -1) && (currIndex <= sizeof(COLUMN_CHARS))) {
        if (COLUMN_CHARS[currIndex] == charVal) 
            indexVal = currIndex;
        currIndex++;
    }
    // returns -1 if not found
    return indexVal;
}

bool Lexer::isAcceptingState(int stateToCheck) {
    int currIndex = 0; // index currently being checked
    bool acceptingState = false; // is the state an accepting state

    // while state is not accepting and current index is within the accepting state array
    while (!acceptingState && currIndex <= sizeof(ACCEPTING_STATES)) {
        if (ACCEPTING_STATES[currIndex] == stateToCheck)
            acceptingState = true;
        currIndex++;
    }
    // returns false if not accepting state
    return acceptingState;
}

bool Lexer::isCompleteToken(char currChar, char nextChar) {
    bool isToken = false; // tracks if char completes token
    buffer += currChar; // add character to buffer
    int currColumnVal = findIndex(currChar); // index of curr char in matrix
    int nextColumnVal = findIndex(nextChar); // index of next char in matrix

    if (isQuotes) {
        // everything in quotes is either a char or space if a valid symbol
        maxBuffer = 1;
    }

    // the next state
    int nextState;
    if (currColumnVal == -1) { // invalid symbol
        nextState = 0;
        logger.error(name, 0, std::string(1, currChar));
    }
    else nextState = TRANS_TABLE[currState][currColumnVal];

    // the next state of the next char based on current transition
    int nextCharNextState;
    if (nextColumnVal == -1) nextCharNextState = 0; // invalid symbol 
    else nextCharNextState = TRANS_TABLE[nextState][nextColumnVal];

    bool acceptingState = isAcceptingState(nextState); // check if next state is accepting

    // end of token either by state
    if (nextCharNextState == 0) {
        // completed the token
        if (acceptingState) {
            isToken = true;
        }
    }

    // end of token early from max buffer
    if (buffer.length() == maxBuffer) {
        isToken = true;
    }

    currState = nextState; // set the current state as the next state

    // only returns true if a completed token
    return isToken;
}

std::string Lexer::getTokenValue() {
    return buffer;
}

std::string Lexer::getToken() {
    // uses the current state we are on
    switch(currState) {
        case 2:
        case 4:
        case 9:
        case 14:
        case 18:
        case 23:
        case 31:
        case 38:
            if (isQuotes) 
                return "CHAR";
            return "ID";
            break;
        case 3:
            return "IF";
            break;
        case 8:
            return "WHILE";
            break;
        case 13:
            return "PRINT";
            break;
        case 17:
        case 22:
            return "BOOLVAL";
            break;
        case 28:
        case 30:
        case 37:
            return "TYPE";
            break;
        case 39:
            return "SPACE";
            break;
        case 40:
            return "DIGIT";
            break;
        case 41:
            return "ASSIGN";
            break;
        case 43:
            return "BOOLOP";
            break;
        case 44:
            return "INTOP";
            break;
        case 45:
            return "LBRACE";
            break;
        case 46:
            return "RBRACE";
            break;
        case 47:
            return "LPAREN";
            break;
        case 48:
            return "RPAREN";
            break;
        case 49:
            // toggle quotes
            isQuotes = !isQuotes;
            return "QUOTMARK";
            break;
        case 50:
            return "EOP";
        default:
            // try again with one less
            if (buffer.length() > 1) {
                maxBuffer = buffer.length() - 1;
                logger.test(name, "max buffer" + std::to_string(maxBuffer));
                return "TRYAGAIN";
            }
            else {
                logger.error(name, 1, buffer);
                return "\033[31mERROR\033[0m";
            }
            break;
    }
}