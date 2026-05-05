#include "SyntaxAnalyzer.h"
#include <algorithm>


SyntaxAnalyzer::SyntaxAnalyzer(const std::vector<Token>& toks, ErrorManager* em)
    : tokens(toks), pos(0), errMgr(em) {}


const Token& SyntaxAnalyzer::actual() const {
    return tokens[pos];
}

const Token& SyntaxAnalyzer::anterior() const {
    return tokens[pos > 0 ? pos - 1 : 0];
}

bool SyntaxAnalyzer::isAtEnd() const {
    return actual().tipo == TokenType::FIN_ARCHIVO;
}

Token SyntaxAnalyzer::avanzar() {
    if (!isAtEnd()) pos++;
    return anterior();
}

bool SyntaxAnalyzer::consumir(TokenType esperado,
                               const std::string& descripcionError) {
    if (actual().tipo == esperado) {
        avanzar();
        return true;
    }
    errMgr->agregarError(
        actual().lexema,
        TipoError::SINTACTICO,
        descripcionError + " Se encontro: '" + actual().lexema + "'.",
        actual().linea,
        actual().columna
    );
    return false;
}

void SyntaxAnalyzer::sincronizar(const std::vector<TokenType>& siguientes) {
    while (!isAtEnd()) {
        TokenType t = actual().tipo;
        for (TokenType s : siguientes) {
            if (t == s) return;
        }
        avanzar();
    }
}

NodoPtr SyntaxAnalyzer::parsear() {
    if (isAtEnd()) return nullptr;
    return parsePrograma();
}


NodoPtr SyntaxAnalyzer::parsePrograma() {
    NodoPtr nodo = nodoNoTerminal("<programa>");


    if (!consumir(TokenType::TABLERO,
        "Se esperaba la palabra reservada 'TABLERO' al inicio del archivo."))
        sincronizar({ TokenType::COLUMNA, TokenType::FIN_ARCHIVO });
    else
        nodo->agregarHijo(nodoTerminal("TABLERO"));


    if (!consumir(TokenType::CADENA,
        "Se esperaba el nombre del tablero como cadena entre comillas despues de 'TABLERO'."))
        sincronizar({ TokenType::LLAVE_AB, TokenType::COLUMNA });
    else
        nodo->agregarHijo(nodoTerminal(anterior().lexema));


    if (!consumir(TokenType::LLAVE_AB,
        "Se esperaba '{' despues del nombre del tablero."))
        sincronizar({ TokenType::COLUMNA });
    else
        nodo->agregarHijo(nodoTerminal("{"));


    nodo->agregarHijo(parseColumnas());

    if (!consumir(TokenType::LLAVE_CL,
        "Se esperaba '}' para cerrar el bloque TABLERO."))
        sincronizar({ TokenType::PUNTO_COMA, TokenType::FIN_ARCHIVO });
    else
        nodo->agregarHijo(nodoTerminal("}"));

    consumir(TokenType::PUNTO_COMA,
        "Se esperaba ';' despues de '}' al cerrar el TABLERO.");
    nodo->agregarHijo(nodoTerminal(";"));

    return nodo;
}

NodoPtr SyntaxAnalyzer::parseColumnas() {
    NodoPtr nodo = nodoNoTerminal("<columnas>");

    if (actual().tipo != TokenType::COLUMNA) {
        errMgr->agregarError(
            actual().lexema,
            TipoError::SINTACTICO,
            "Se esperaba al menos una seccion COLUMNA dentro del TABLERO.",
            actual().linea, actual().columna
        );
        return nodo;
    }

    while (!isAtEnd() && actual().tipo == TokenType::COLUMNA) {
        nodo->agregarHijo(parseColumna());
    }

    return nodo;
}

NodoPtr SyntaxAnalyzer::parseColumna() {
    NodoPtr nodo = nodoNoTerminal("<columna>");

    consumir(TokenType::COLUMNA, "Se esperaba 'COLUMNA'.");
    nodo->agregarHijo(nodoTerminal("COLUMNA"));


    if (!consumir(TokenType::CADENA,
        "Se esperaba el nombre de la columna como cadena entre comillas."))
        sincronizar({ TokenType::LLAVE_AB, TokenType::TAREA });
    else
        nodo->agregarHijo(nodoTerminal(anterior().lexema));

    if (!consumir(TokenType::LLAVE_AB,
        "Se esperaba '{' despues del nombre de la columna."))
        sincronizar({ TokenType::TAREA });
    else
        nodo->agregarHijo(nodoTerminal("{"));

    nodo->agregarHijo(parseListaTareas());

    if (!consumir(TokenType::LLAVE_CL,
        "Se esperaba '}' para cerrar el bloque COLUMNA."))
        sincronizar({ TokenType::PUNTO_COMA, TokenType::COLUMNA, TokenType::LLAVE_CL });
    else
        nodo->agregarHijo(nodoTerminal("}"));

    consumir(TokenType::PUNTO_COMA,
        "Se esperaba ';' despues de '}' al cerrar la COLUMNA.");
    nodo->agregarHijo(nodoTerminal(";"));

    return nodo;
}

NodoPtr SyntaxAnalyzer::parseListaTareas() {
    NodoPtr nodo = nodoNoTerminal("<lista_tareas>");

    if (actual().tipo != TokenType::TAREA) {
        errMgr->agregarError(
            actual().lexema,
            TipoError::SINTACTICO,
            "Se esperaba al menos una 'tarea' dentro de la COLUMNA.",
            actual().linea, actual().columna
        );
        return nodo;
    }

    nodo->agregarHijo(parseTarea());

    while (!isAtEnd() && actual().tipo == TokenType::COMA) {
        if (pos + 1 < (int)tokens.size() &&
            tokens[pos + 1].tipo == TokenType::TAREA) {
            nodo->agregarHijo(nodoTerminal(","));
            avanzar(); 
            nodo->agregarHijo(parseTarea());
        } else {
            break;
        }
    }

    return nodo;
}

NodoPtr SyntaxAnalyzer::parseTarea() {
    NodoPtr nodo = nodoNoTerminal("<tarea>");

    consumir(TokenType::TAREA, "Se esperaba la palabra reservada 'tarea'.");
    nodo->agregarHijo(nodoTerminal("tarea"));

    if (!consumir(TokenType::DOS_PUNTOS,
        "Se esperaba ':' despues de 'tarea'."))
        sincronizar({ TokenType::CADENA, TokenType::CORCHETE_AB });
    else
        nodo->agregarHijo(nodoTerminal(":"));

    if (!consumir(TokenType::CADENA,
        "Se esperaba el nombre de la tarea como cadena entre comillas."))
        sincronizar({ TokenType::CORCHETE_AB });
    else
        nodo->agregarHijo(nodoTerminal(anterior().lexema));

    if (!consumir(TokenType::CORCHETE_AB,
        "Se esperaba '[' para iniciar los atributos de la tarea."))
        sincronizar({ TokenType::PRIORIDAD, TokenType::RESPONSABLE,
                      TokenType::FECHA_LIMITE, TokenType::CORCHETE_CL });
    else
        nodo->agregarHijo(nodoTerminal("["));

    nodo->agregarHijo(parseListaAtributos());

    if (!consumir(TokenType::CORCHETE_CL,
        "Se esperaba ']' para cerrar los atributos de la tarea."))
        sincronizar({ TokenType::COMA, TokenType::LLAVE_CL });
    else
        nodo->agregarHijo(nodoTerminal("]"));

    return nodo;
}

NodoPtr SyntaxAnalyzer::parseListaAtributos() {
    NodoPtr nodo = nodoNoTerminal("<lista_attrs>");


    auto esAtributo = [&]() {
        TokenType t = actual().tipo;
        return t == TokenType::PRIORIDAD   ||
               t == TokenType::RESPONSABLE ||
               t == TokenType::FECHA_LIMITE;
    };

    if (!esAtributo()) {
        errMgr->agregarError(
            actual().lexema,
            TipoError::SINTACTICO,
            "Se esperaba al menos un atributo (prioridad, responsable o fecha_limite).",
            actual().linea, actual().columna
        );
        return nodo;
    }

    nodo->agregarHijo(parseAtributo());

    while (!isAtEnd() && actual().tipo == TokenType::COMA) {
        if (pos + 1 < (int)tokens.size()) {
            TokenType sig = tokens[pos + 1].tipo;
            if (sig == TokenType::PRIORIDAD  ||
                sig == TokenType::RESPONSABLE ||
                sig == TokenType::FECHA_LIMITE) {
                nodo->agregarHijo(nodoTerminal(","));
                avanzar(); // consume ','
                nodo->agregarHijo(parseAtributo());
            } else {
                break; // coma antes de ']', no es un error de lista de attrs
            }
        } else {
            break;
        }
    }

    return nodo;
}

NodoPtr SyntaxAnalyzer::parseAtributo() {
    NodoPtr nodo = nodoNoTerminal("<atributo>");

    TokenType t = actual().tipo;

    if (t == TokenType::PRIORIDAD) {
        nodo->agregarHijo(nodoTerminal("prioridad"));
        avanzar();
        consumir(TokenType::DOS_PUNTOS,
            "Se esperaba ':' despues de 'prioridad'.");
        nodo->agregarHijo(nodoTerminal(":"));
        nodo->agregarHijo(parseValorPrioridad());
    }
    else if (t == TokenType::RESPONSABLE) {
        nodo->agregarHijo(nodoTerminal("responsable"));
        avanzar();
        consumir(TokenType::DOS_PUNTOS,
            "Se esperaba ':' despues de 'responsable'.");
        nodo->agregarHijo(nodoTerminal(":"));
        if (!consumir(TokenType::CADENA,
            "Se esperaba una cadena como valor de 'responsable'."))
            sincronizar({ TokenType::COMA, TokenType::CORCHETE_CL });
        else
            nodo->agregarHijo(nodoTerminal(anterior().lexema));
    }
    else if (t == TokenType::FECHA_LIMITE) {
        nodo->agregarHijo(nodoTerminal("fecha_limite"));
        avanzar();
        consumir(TokenType::DOS_PUNTOS,
            "Se esperaba ':' despues de 'fecha_limite'.");
        nodo->agregarHijo(nodoTerminal(":"));
        if (!consumir(TokenType::FECHA,
            "Se esperaba una fecha en formato AAAA-MM-DD como valor de 'fecha_limite'."))
            sincronizar({ TokenType::COMA, TokenType::CORCHETE_CL });
        else
            nodo->agregarHijo(nodoTerminal(anterior().lexema));
    }
    else {
        errMgr->agregarError(
            actual().lexema,
            TipoError::SINTACTICO,
            "Atributo desconocido '" + actual().lexema +
            "'. Se esperaba prioridad, responsable o fecha_limite.",
            actual().linea, actual().columna
        );
        avanzar(); 
    }

    return nodo;
}


NodoPtr SyntaxAnalyzer::parseValorPrioridad() {
    NodoPtr nodo = nodoNoTerminal("<valor_prioridad>");

    TokenType t = actual().tipo;
    if (t == TokenType::ALTA ||
        t == TokenType::MEDIA ||
        t == TokenType::BAJA) {
        nodo->agregarHijo(nodoTerminal(actual().lexema));
        avanzar();
    } else {
        errMgr->agregarError(
            actual().lexema,
            TipoError::SINTACTICO,
            "Se esperaba ALTA, MEDIA o BAJA como valor de prioridad. "
            "Se encontro: '" + actual().lexema + "'.",
            actual().linea, actual().columna
        );
        sincronizar({ TokenType::COMA, TokenType::CORCHETE_CL });
    }

    return nodo;
}
