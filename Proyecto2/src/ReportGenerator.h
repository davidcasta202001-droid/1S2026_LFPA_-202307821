#pragma once
#include <string>
#include <vector>
#include "Token.h"
#include "ErrorManager.h"
#include "NodoArbol.h"


struct Tarea {
    std::string nombre;
    std::string prioridad;   
    std::string responsable;
    std::string fechaLimite;
};

struct Columna {
    std::string         nombre;
    std::vector<Tarea>  tareas;
};

struct Tablero {
    std::string            nombre;
    std::vector<Columna>   columnas;
};


class ReportGenerator {
public:
    ReportGenerator(const NodoPtr& arbol,
                    const std::vector<Token>& tokens,
                    const ErrorManager& errMgr);

    bool generarTodos(const std::string& carpeta);

    bool generarKanban        (const std::string& ruta);
    bool generarResponsables  (const std::string& ruta);
    bool generarErrores       (const std::string& ruta);
    bool generarDot           (const std::string& ruta);

private:
    NodoPtr                    arbol;
    std::vector<Token>         tokens;
    const ErrorManager&        errMgr;
    Tablero                    tablero; 

    void extraerDatos();
    void extraerColumna(const NodoPtr& nodoColumna, Columna& col);
    void extraerTarea  (const NodoPtr& nodoTarea,   Tarea&   t);

    int  dotContador;
    void dotNodo(std::string& out, const NodoPtr& nodo, int idPadre = -1);


    static std::string htmlHeader(const std::string& titulo,
                                  const std::string& estilosExtra = "");
    static std::string htmlFooter();
    static std::string colorPrioridad(const std::string& p); 
    static std::string badgePrioridad(const std::string& p); 
    static bool        escribirArchivo(const std::string& ruta,
                                       const std::string& contenido);
};
