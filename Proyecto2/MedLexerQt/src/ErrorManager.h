#ifndef ERRORMANAGER_H
#define ERRORMANAGER_H

#include <vector>
#include <string>

struct Error {
    std::string lexeme;
    std::string description;
    int line;
    int column;
};

class ErrorManager {
public:
    std::vector<Error> errors;

    void addError(std::string lexeme, std::string desc, int line, int column);
    void printErrors();
};

#endif