#include "AnalizadorLexico.h"
#include <cctype>


LexicalAnalyzer::LexicalAnalyzer(std::string input)
    : input(input), pos(0), line(1), column(1) {}

char LexicalAnalyzer::currentChar()
{
    if (pos >= input.size())
        return '\0';
    return input[pos];
}
std::vector<Error> LexicalAnalyzer::getErrores() {
    return errorManager.errors;
}
void LexicalAnalyzer::advance()
{
    if (currentChar() == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }
    pos++;
}

void LexicalAnalyzer::skipWhitespace()
{
    while (isspace(currentChar()))
        advance();
}

Token LexicalAnalyzer::nextToken()
{
    skipWhitespace();

    int startCol = column;

    if (currentChar() == '\0')
        return Token(TokenType::END, "", line, column);

    // IDENTIFIERS / KEYWORDS
    if (isalpha(currentChar()))
    {
        std::string lexeme;

        while (isalnum(currentChar()) || currentChar() == '_')
        {
            lexeme += currentChar();
            advance();
        }

        if (lexeme == "HOSPITAL")
            return Token(TokenType::HOSPITAL, lexeme, line, startCol);
        if (lexeme == "PACIENTES")
            return Token(TokenType::PACIENTES, lexeme, line, startCol);
        if (lexeme == "MEDICOS")
            return Token(TokenType::MEDICOS, lexeme, line, startCol);
        if (lexeme == "CITAS")
            return Token(TokenType::CITAS, lexeme, line, startCol);
        if (lexeme == "DIAGNOSTICOS")
            return Token(TokenType::DIAGNOSTICOS, lexeme, line, startCol);

        return Token(TokenType::IDENTIFIER, lexeme, line, startCol);
    }

    // STRING
    if (currentChar() == '"')
    {
        std::string lexeme;
        advance();

        while (currentChar() != '"' && currentChar() != '\0')
        {
            lexeme += currentChar();
            advance();
        }

        if (currentChar() == '"')
        {
            advance();
            return Token(TokenType::STRING, lexeme, line, startCol);
        }
        else
        {
            errorManager.addError(lexeme, "Cadena sin cerrar", line, startCol);
            return Token(TokenType::ERROR, lexeme, line, startCol);
        }
    }

    // NUMBER
    if (isdigit(currentChar()))
    {
        std::string lexeme;

        while (isdigit(currentChar()))
        {
            lexeme += currentChar();
            advance();
        }

        return Token(TokenType::NUMBER, lexeme, line, startCol);
    }

    char c = currentChar();

    if (c == '[' || c == ']' || c == ',' )
    {
        advance();
        return Token(TokenType::SYMBOL, std::string(1, c), line, startCol);
    }


    std::string lexeme(1, c);
    errorManager.addError(lexeme, "Caracter no reconocido", line, startCol);

    advance();
    return Token(TokenType::ERROR, lexeme, line, startCol);
}