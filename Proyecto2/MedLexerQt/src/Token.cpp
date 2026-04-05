#include "Token.h"

Token::Token(TokenType type, std::string lexeme, int line, int column)
    : type(type), lexeme(lexeme), line(line), column(column) {}

std::string Token::toString() const {
    return lexeme + " (" + std::to_string(line) + ":" + std::to_string(column) + ")";
}

std::string tokenTypeToString(TokenType type)
{
    switch (type)
    {
    case TokenType::HOSPITAL: return "HOSPITAL";
    case TokenType::PACIENTES: return "PACIENTES";
    case TokenType::MEDICOS: return "MEDICOS";
    case TokenType::CITAS: return "CITAS";
    case TokenType::DIAGNOSTICOS: return "DIAGNOSTICOS";
    case TokenType::STRING: return "STRING";
    case TokenType::NUMBER: return "NUMBER";
    case TokenType::IDENTIFIER: return "IDENTIFIER";
    case TokenType::SYMBOL: return "SYMBOL";
    case TokenType::ERROR: return "ERROR";
    case TokenType::END: return "END";
    default: return "UNKNOWN";
    }
}