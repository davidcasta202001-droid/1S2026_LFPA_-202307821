#pragma once
#include <vector>
#include "Token.h"
#include "ErrorManager.h"
#include "NodoArbol.h"


class SyntaxAnalyzer {
public:
    SyntaxAnalyzer(const std::vector<Token>& tokens, ErrorManager* errMgr);

    NodoPtr parsear();

private:

    std::vector<Token> tokens;
    int                pos;      
    ErrorManager*      errMgr;

    const Token& actual() const;
    const Token& anterior() const;
    bool isAtEnd() const;


    bool consumir(TokenType esperado, const std::string& descripcionError);

    Token avanzar();

 
    void sincronizar(const std::vector<TokenType>& siguientes);



    NodoPtr parsePrograma();       
    NodoPtr parseColumnas();        
    NodoPtr parseColumna();       
    NodoPtr parseListaTareas();      
    NodoPtr parseTarea();        
    NodoPtr parseListaAtributos();    
    NodoPtr parseAtributo();         
    NodoPtr parseValorPrioridad();    
};
