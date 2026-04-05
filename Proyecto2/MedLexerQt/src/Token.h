#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum class TokenType {
    // Palabras clave
    HOSPITAL, PACIENTES, MEDICOS, CITAS, DIAGNOSTICOS,
    PACIENTE, MEDICO, CITA, DIAGNOSTICO,

    // Datos
    STRING, NUMBER,
    DATE, TIME,
    ID_CODE,

    // ENUMS
    ESPECIALIDAD,
    DOSIS,
    BLOOD_TYPE,

    // Símbolos
    SYMBOL, LBRACE, RBRACE, LBRACKET, RBRACKET,
    COLON, COMMA, SEMICOLON,

    IDENTIFIER,
    END,
    ERROR
};
std::string tokenTypeToString(TokenType type);
class Token {
public:
    TokenType type;
    std::string lexeme;
    int line;
    int column;

    Token(TokenType type, std::string lexeme, int line, int column);
    std::string toString() const;
};

#endif