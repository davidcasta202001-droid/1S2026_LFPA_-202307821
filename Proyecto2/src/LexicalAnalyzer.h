#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Token.h"
#include "ErrorManager.h"


class LexicalAnalyzer {
public:
    LexicalAnalyzer(const std::string& fuente, ErrorManager* errMgr);

    Token nextToken();

    std::vector<Token> tokenizarTodo();

private:

    std::string fuente;
    int         pos;      
    int         linea;
    int         columna;
    ErrorManager* errMgr;

    static const std::unordered_map<std::string, TokenType> palabrasReservadas;

    char peek() const;                  
    char advance();                     
    bool isAtEnd() const;
    void skipWhitespace();        

    // Ramas del AFD
    Token leerIdentificadorOReservada(int linIni, int colIni);
    Token leerNumeroOFecha(int linIni, int colIni);
    Token leerCadena(int linIni, int colIni);
    Token leerDelimitador(char c, int linIni, int colIni);

    Token errorToken(const std::string& lexema, const std::string& desc,
                     int linIni, int colIni);
};
