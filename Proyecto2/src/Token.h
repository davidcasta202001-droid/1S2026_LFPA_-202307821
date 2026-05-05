#pragma once
#include <string>

enum class TokenType {
    TABLERO,
    COLUMNA,

    TAREA,
    PRIORIDAD,
    RESPONSABLE,
    FECHA_LIMITE,

    ALTA,
    MEDIA,
    BAJA,

    CADENA,     
    ENTERO,    
    FECHA,    


    LLAVE_AB,  
    LLAVE_CL,   
    CORCHETE_AB,
    CORCHETE_CL,
    DOS_PUNTOS, 
    COMA,       
    PUNTO_COMA, 

    FIN_ARCHIVO,
    ERROR     
};

inline std::string tokenTypeToString(TokenType t) {
    switch (t) {
        case TokenType::TABLERO:      return "TABLERO";
        case TokenType::COLUMNA:      return "COLUMNA";
        case TokenType::TAREA:        return "TAREA";
        case TokenType::PRIORIDAD:    return "PRIORIDAD";
        case TokenType::RESPONSABLE:  return "RESPONSABLE";
        case TokenType::FECHA_LIMITE: return "FECHA_LIMITE";
        case TokenType::ALTA:         return "ALTA";
        case TokenType::MEDIA:        return "MEDIA";
        case TokenType::BAJA:         return "BAJA";
        case TokenType::CADENA:       return "CADENA";
        case TokenType::ENTERO:       return "ENTERO";
        case TokenType::FECHA:        return "FECHA";
        case TokenType::LLAVE_AB:     return "LLAVE_AB";
        case TokenType::LLAVE_CL:     return "LLAVE_CL";
        case TokenType::CORCHETE_AB:  return "CORCHETE_AB";
        case TokenType::CORCHETE_CL:  return "CORCHETE_CL";
        case TokenType::DOS_PUNTOS:   return "DOS_PUNTOS";
        case TokenType::COMA:         return "COMA";
        case TokenType::PUNTO_COMA:   return "PUNTO_COMA";
        case TokenType::FIN_ARCHIVO:  return "FIN_ARCHIVO";
        case TokenType::ERROR:        return "ERROR";
        default:                      return "DESCONOCIDO";
    }
}


struct Token {
    TokenType   tipo;
    std::string lexema;   
    int         linea;
    int         columna;

    Token(TokenType t, const std::string& lex, int lin, int col)
        : tipo(t), lexema(lex), linea(lin), columna(col) {}
};
