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

    // while state is not accepting and current index is within the accpeting state array
    while (!acceptingState && currIndex <= sizeof(ACCEPTING_STATES)) {
        if (ACCEPTING_STATES[currIndex] == stateToCheck)
            acceptingState = true;
        currIndex++;
    }
    // returns false if not accepting state
    return acceptingState;
}

bool Lexer::isCompleteToken(char currChar) {
    bool isToken = false; // tracks if char completes token
    buffer += currChar; // add character to buffer

    int columnVal = findIndex(currChar); // get index of character in matrix
    // check if the value is in the grammar
    // if (columnVal == -1) return error; 

    int nextState = TRANS_TABLE[currState][columnVal]; // get the next state
    bool acceptingState = isAcceptingState(nextState); // check if current state is accepting

    // if the next state is an accepting state
    if (acceptingState) {
        isToken = true; // we have a complete token
        // else we have an error
    } 
    // set the current state as the next state
    currState = nextState;

    // only returns true if a completed token
    return isToken;
}

std::string Lexer::getTokenValue() {
    return buffer;
}

std::string Lexer::getToken() {
    // uses the current state we are on
    switch(currState) {
        case 1:
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
        default:
            return "ERROR";
            break;
    }
}