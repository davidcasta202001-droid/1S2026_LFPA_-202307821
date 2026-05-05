#include "LexicalAnalyzer.h"
#include <cctype>
#include <stdexcept>


const std::unordered_map<std::string, TokenType>
LexicalAnalyzer::palabrasReservadas = {
    { "TABLERO",      TokenType::TABLERO      },
    { "COLUMNA",      TokenType::COLUMNA      },
    { "tarea",        TokenType::TAREA        },
    { "prioridad",    TokenType::PRIORIDAD    },
    { "responsable",  TokenType::RESPONSABLE  },
    { "fecha_limite", TokenType::FECHA_LIMITE },
    { "ALTA",         TokenType::ALTA         },
    { "MEDIA",        TokenType::MEDIA        },
    { "BAJA",         TokenType::BAJA         },
};

LexicalAnalyzer::LexicalAnalyzer(const std::string& src, ErrorManager* em)
    : fuente(src), pos(0), linea(1), columna(1), errMgr(em) {}


char LexicalAnalyzer::peek() const {
    if (isAtEnd()) return '\0';
    return fuente[pos];
}

char LexicalAnalyzer::advance() {
    char c = fuente[pos++];
    if (c == '\n') { linea++; columna = 1; }
    else           { columna++; }
    return c;
}

bool LexicalAnalyzer::isAtEnd() const {
    return pos >= (int)fuente.size();
}

void LexicalAnalyzer::skipWhitespace() {
    while (!isAtEnd()) {
        char c = peek();
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
            advance();
        else
            break;
    }
}

Token LexicalAnalyzer::nextToken() {
    skipWhitespace();  

    if (isAtEnd())
        return Token(TokenType::FIN_ARCHIVO, "EOF", linea, columna);

    int linIni = linea;
    int colIni = columna;
    char c = peek();

    if (std::isalpha((unsigned char)c) || c == '_')
        return leerIdentificadorOReservada(linIni, colIni);

    if (std::isdigit((unsigned char)c))
        return leerNumeroOFecha(linIni, colIni);

    if (c == '"') {
        advance(); 
        return leerCadena(linIni, colIni);
    }

    if (c == '{' || c == '}' || c == '[' || c == ']' ||
        c == ':' || c == ',' || c == ';') {
        advance();
        return leerDelimitador(c, linIni, colIni);
    }

    advance();
    std::string lex(1, c);
    return errorToken(lex,
        "Caracter no reconocido '" + lex + "'.",
        linIni, colIni);
}


std::vector<Token> LexicalAnalyzer::tokenizarTodo() {
    std::vector<Token> tokens;
    Token t = nextToken();
    while (t.tipo != TokenType::FIN_ARCHIVO) {
        tokens.push_back(t);
        t = nextToken();
    }
    tokens.push_back(t); 
    return tokens;
}


Token LexicalAnalyzer::leerIdentificadorOReservada(int linIni, int colIni) {
    std::string lexema;

    while (!isAtEnd()) {
        char c = peek();
        if (std::isalnum((unsigned char)c) || c == '_')
            lexema += advance();
        else
            break;
    }
    auto it = palabrasReservadas.find(lexema);
    if (it != palabrasReservadas.end())
        return Token(it->second, lexema, linIni, colIni);

    return Token(TokenType::ERROR, lexema, linIni, colIni);
}


Token LexicalAnalyzer::leerNumeroOFecha(int linIni, int colIni) {
    std::string lexema;

    while (!isAtEnd() && std::isdigit((unsigned char)peek()))
        lexema += advance();

  
    if (lexema.size() == 4 && !isAtEnd() && peek() == '-') {
        std::string tentativa = lexema;
        tentativa += advance(); 


        int mesDigitos = 0;
        while (!isAtEnd() && std::isdigit((unsigned char)peek()) && mesDigitos < 2) {
            tentativa += advance();
            mesDigitos++;
        }

        if (mesDigitos == 2 && !isAtEnd() && peek() == '-') {
            tentativa += advance();

            int diaDigitos = 0;
            while (!isAtEnd() && std::isdigit((unsigned char)peek()) && diaDigitos < 2) {
                tentativa += advance();
                diaDigitos++;
            }

            if (diaDigitos == 2) {
                return Token(TokenType::FECHA, tentativa, linIni, colIni);
            }

            return errorToken(tentativa,
                "Formato de fecha invalido. Se esperaba AAAA-MM-DD.",
                linIni, colIni);
        }
        return errorToken(tentativa,
            "Formato de fecha invalido. Se esperaba AAAA-MM-DD.",
            linIni, colIni);
    }

    return Token(TokenType::ENTERO, lexema, linIni, colIni);
}
Token LexicalAnalyzer::leerCadena(int linIni, int colIni) {
    std::string contenido;

    while (!isAtEnd()) {
        char c = peek();

        if (c == '"') {
            advance(); 
            return Token(TokenType::CADENA, "\"" + contenido + "\"", linIni, colIni);
        }

        if (c == '\n') {
            return errorToken("\"" + contenido,
                "Cadena no cerrada antes del fin de linea.",
                linIni, colIni);
        }

        contenido += advance();
    }

    return errorToken("\"" + contenido,
        "Cadena no cerrada antes del fin del archivo.",
        linIni, colIni);
}

Token LexicalAnalyzer::leerDelimitador(char c, int linIni, int colIni) {
    std::string lex(1, c);
    switch (c) {
        case '{': return Token(TokenType::LLAVE_AB,      lex, linIni, colIni);
        case '}': return Token(TokenType::LLAVE_CL,      lex, linIni, colIni);
        case '[': return Token(TokenType::CORCHETE_AB,   lex, linIni, colIni);
        case ']': return Token(TokenType::CORCHETE_CL,   lex, linIni, colIni);
        case ':': return Token(TokenType::DOS_PUNTOS,    lex, linIni, colIni);
        case ',': return Token(TokenType::COMA,          lex, linIni, colIni);
        case ';': return Token(TokenType::PUNTO_COMA,    lex, linIni, colIni);
        default:  return errorToken(lex, "Delimitador desconocido.", linIni, colIni);
    }
}


Token LexicalAnalyzer::errorToken(const std::string& lexema,
                                  const std::string& desc,
                                  int linIni, int colIni) {
    errMgr->agregarError(lexema, TipoError::LEXICO, desc, linIni, colIni);
    return Token(TokenType::ERROR, lexema, linIni, colIni);
}
