#ifndef ANALIZADORLEXICO_H
#define ANALIZADORLEXICO_H

#include <string>
#include "Token.h"
#include "ErrorManager.h"
#include <vector>
#include "ErrorManager.h"

class LexicalAnalyzer {
private:
    std::string input;
    int pos;
    int line;
    int column;

    char currentChar();
    void advance();
    void skipWhitespace();

public:
    ErrorManager errorManager;
    std::vector<Error> getErrores();
    LexicalAnalyzer(std::string input);

    Token nextToken();
};

#endif