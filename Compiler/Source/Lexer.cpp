#include "../Headers/Lexer.hpp"

Lexer::Lexer() {
    buffer = ""; // set buffer to empty
    currState = 1; // start at first state
}

void Lexer::clearBuffer() {
    buffer = ""; // set buffer to empty
}

void Lexer::resetState() {
    currState = 1; // reset the state
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

    // the next state
    int nextState;
    if (currColumnVal == -1) nextState = 0; // error (this is not a token)
    else nextState = TRANS_TABLE[currState][currColumnVal];

    // the next state of the next char based on current transition
    int nextCharNextState;
    if (nextColumnVal == -1) nextCharNextState = 0; // error (make different error as this is not in the table)
    else nextCharNextState = TRANS_TABLE[nextState][nextColumnVal];

    bool acceptingState = isAcceptingState(nextState); // check if next state is accepting

    // end of token
    if (nextCharNextState == 0) {
        // valid token
        if (acceptingState)
            isToken = true;
        // else raise a flag
    }

    // if the next state is an accepting state
    //if (acceptingState) {
    //    if (nextCharNextState == 0) // end of the token
    //        isToken = true; // we have a complete token
        // else we have an error
    //} 
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
        case 29:
        case 31:
        case 38:
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
            return "CHARLIST";
            break;
        default:
            return "ERROR";
            break;
    }
}