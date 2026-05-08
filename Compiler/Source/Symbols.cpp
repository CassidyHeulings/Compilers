#include "../Headers/Symbols.hpp"
#include <iterator>

Symbols::Symbols() {
    // initialize each element in array
    for (int i = 0; i < sizeof(symbols); i++) {
        symbols[i] = {'#', "none", false, false};
    }
}

int Symbols::getIndex(char letter) {
    // a ascii = 97 -> index 0 in array
    return (int)letter - 97;
}

bool Symbols::contains(char checkId) {
    // check if symbol at the index of the id is already created
    return symbols[getIndex(checkId)].id != '#';
}

void Symbols::addSymbol(char newId) {
    // check for collision
    if (contains(newId)) return; // error
    // safe to insert the symbol
    symbols[getIndex(newId)].id = newId;
}

void Symbols::setType(char id, const std::string& newType) {
    // check if symbol already has a type
    if (symbols[getIndex(id)].type != "none")
        // if symbol is already set to the type we are trying to set it to
        if (symbols[getIndex(id)].type == newType) return; // warning?
        else return; // error - not same type
    // safe to set new type
    symbols[getIndex(id)].type = newType;
}

void Symbols::setInit(char id) {
    if (!contains(id)) {
        // add the symbol into the array
        addSymbol(id);
        // warning? error? did not create with type
    }
    symbols[getIndex(id)].isInit = true;
}

void Symbols::setUsed(char id) {
    if (!contains(id)) {
        // add the symbol into the array
        addSymbol(id);
        // warning? error? did not create with type
    }
    symbols[getIndex(id)].isUsed = true;
}