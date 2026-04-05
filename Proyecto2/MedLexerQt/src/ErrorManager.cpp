#include "ErrorManager.h"
#include <iostream>

void ErrorManager::addError(std::string lexeme, std::string desc, int line, int column) {
    errors.push_back({lexeme, desc, line, column});
}

void ErrorManager::printErrors() {
    for (auto &e : errors) {
        std::cout << "Error: " << e.lexeme 
                  << " (" << e.line << ":" << e.column << ") -> "
                  << e.description << std::endl;
    }
}