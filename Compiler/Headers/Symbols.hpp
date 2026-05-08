#include <string>
#pragma once

class Symbols {
public:
    Symbols();
    // check if id already exists
    bool contains(char checkId);
    // add a symbol to the symbols array
    void addSymbol(char newId);
    // set the type of an id
    void setType(char id, const std::string& newType);
    // id was initialized
    void setInit(char id);
    // id was used
    void setUsed(char id);
    // gets the name of each symbol
    std::vector<char> getSymbols();
    // getters
    std::string getType(char id);
    bool getInit(char id);
    bool getUsed(char id);
private:
    // data for each symbol in the hash table
    struct Symbol {
        // variable name and the key of the symbol
        char id;
        // variable type
        std::string type;
        // whether variable is initialized
        bool isInit;
        // whether variable is used
        bool isUsed;
    };
    // array holding the symbols in the scope node
    Symbol symbols[26];
    // turn char into index in array
    int getIndex(char letter);
};