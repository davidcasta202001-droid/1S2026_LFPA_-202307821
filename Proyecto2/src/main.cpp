/*#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "ErrorManager.h"
#include "ReportGenerator.h"

void imprimirArbol(const NodoPtr &nodo, const std::string &pref = "", bool esUltimo = true)
{
    if (!nodo)
        return;
    std::cout << pref << (esUltimo ? "└── " : "├── ");
    if (nodo->esTerminal)
        std::cout << "[" << nodo->etiqueta << "]\n";
    else
        std::cout << nodo->etiqueta << "\n";
    std::string np = pref + (esUltimo ? "    " : "│   ");
    for (size_t i = 0; i < nodo->hijos.size(); i++)
        imprimirArbol(nodo->hijos[i], np, i == nodo->hijos.size() - 1);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Uso: taskscript <archivo.task> [carpeta_reportes]\n";
        return 1;
    }

    std::ifstream archivo(argv[1]);
    if (!archivo.is_open())
    {
        std::cerr << "No se pudo abrir: " << argv[1] << "\n";
        return 1;
    }
    std::ostringstream ss;
    ss << archivo.rdbuf();
    std::string fuente = ss.str();

    // ── Análisis léxico ──────────────────────────
    ErrorManager errMgr;
    LexicalAnalyzer lexer(fuente, &errMgr);
    auto tokens = lexer.tokenizarTodo();

    std::cout << "\n========== TABLA DE TOKENS (" << tokens.size() - 1 << " tokens) ==========\n";
    std::cout << "No.  | Tipo              | Lexema               | Lin | Col\n";
    std::cout << "-----|-------------------|----------------------|-----|----\n";
    int num = 1;
    for (auto &t : tokens)
    {
        if (t.tipo == TokenType::FIN_ARCHIVO)
            break;
        printf("%-4d | %-17s | %-20s | %-3d | %d\n",
               num++, tokenTypeToString(t.tipo).c_str(),
               t.lexema.c_str(), t.linea, t.columna);
    }

    // ── Análisis sintáctico ──────────────────────
    SyntaxAnalyzer parser(tokens, &errMgr);
    NodoPtr raiz = parser.parsear();

    std::cout << "\n========== ARBOL DE DERIVACION ==========\n";
    imprimirArbol(raiz);

    // ── Errores ──────────────────────────────────
    if (errMgr.hayErrores())
    {
        std::cout << "\n========== ERRORES (" << errMgr.obtenerErrores().size() << ") ==========\n";
        for (auto &e : errMgr.obtenerErrores())
            printf("[%s] Lin %d Col %d — %s: %s\n",
                   e.tipoStr().c_str(), e.linea, e.columna,
                   e.lexema.c_str(), e.descripcion.c_str());
    }
    else
    {
        std::cout << "\nAnalisis completado sin errores.\n";
    }

    // ── Reportes HTML + DOT ──────────────────────
    std::string carpeta = (argc >= 3) ? argv[2] : "./reportes";
    mkdir(carpeta.c_str());

    ReportGenerator gen(raiz, tokens, errMgr);
    if (gen.generarTodos(carpeta))
    {
        std::cout << "\nReportes generados en: " << carpeta << "/\n";
        std::cout << "  reporte1_kanban.html\n";
        std::cout << "  reporte2_responsables.html\n";
        std::cout << "  reporte3_errores.html\n";
        std::cout << "  arbol.dot\n";
    }
    else
    {
        std::cerr << "\nError al escribir reportes en: " << carpeta << "\n";
    }
    system("pause");
    return 0;
}
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/stat.h>

#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "ErrorManager.h"
#include "ReportGenerator.h"

void imprimirArbol(const NodoPtr& nodo,
                   const std::string& pref = "",
                   bool esUltimo = true)
{
    if (!nodo) return;
    std::cout << pref << (esUltimo ? "└── " : "├── ");
    if (nodo->esTerminal)
        std::cout << "[" << nodo->etiqueta << "]\n";
    else
        std::cout << nodo->etiqueta << "\n";

    std::string np = pref + (esUltimo ? "    " : "│   ");
    for (size_t i = 0; i < nodo->hijos.size(); i++)
        imprimirArbol(nodo->hijos[i], np, i == nodo->hijos.size() - 1);
}

void separador(const std::string& titulo) {
    std::cout << "\n";
    std::cout << "══════════════════════════════════════════\n";
    std::cout << "  " << titulo << "\n";
    std::cout << "══════════════════════════════════════════\n";
}

void analizar(const std::string& fuente, const std::string& carpetaReportes = "./reportes") {

    separador("1. ANÁLISIS LÉXICO — TABLA DE TOKENS");

    ErrorManager errMgr;
    LexicalAnalyzer lexer(fuente, &errMgr);
    auto tokens = lexer.tokenizarTodo();

    printf("%-4s | %-17s | %-22s | %-5s | %s\n",
        "No.", "Tipo", "Lexema", "Lin", "Col");
    std::cout << "-----|-------------------|------------------------|-------|----\n";

    int num = 1;
    for (const auto& tok : tokens) {
        if (tok.tipo == TokenType::FIN_ARCHIVO) break;
        printf("%-4d | %-17s | %-22s | %-5d | %d\n",
            num++,
            tokenTypeToString(tok.tipo).c_str(),
            tok.lexema.c_str(),
            tok.linea,
            tok.columna);
    }
    std::cout << "\nTotal tokens: " << num - 1 << "\n";

    separador("2. ANÁLISIS SINTÁCTICO — ÁRBOL DE DERIVACIÓN");

    SyntaxAnalyzer parser(tokens, &errMgr);
    NodoPtr raiz = parser.parsear();

    if (raiz)
        imprimirArbol(raiz);
    else
        std::cout << "(árbol vacío — el archivo tiene errores críticos)\n";

    separador("3. TABLA DE ERRORES");

    const auto& errores = errMgr.obtenerErrores();
    if (errores.empty()) {
        std::cout << "  ✔  Sin errores — el archivo es válido.\n";
    } else {
        printf("%-3s | %-20s | %-10s | %-3s | %-3s\n",
            "No.", "Lexema", "Tipo", "Lin", "Col");
        std::cout << "----|----------------------|------------|-----|----\n";
        for (const auto& e : errores) {
            printf("%-3d | %-20s | %-10s | %-3d | %d\n",
                e.numero,
                e.lexema.c_str(),
                e.tipoStr().c_str(),
                e.linea,
                e.columna);
            std::cout << "    Descripción: " << e.descripcion << "\n";
        }
    }

    // ── 4. Generar reportes HTML ─────────────
    separador("4. GENERANDO REPORTES HTML");

    #ifdef _WIN32
        mkdir(carpetaReportes.c_str());
    #else
        mkdir(carpetaReportes.c_str(), 0755);
    #endif

    ReportGenerator gen(raiz, tokens, errMgr);
    if (gen.generarTodos(carpetaReportes)) {
        std::cout << "  Reportes generados en: " << carpetaReportes << "/\n";
        std::cout << "    reporte1_kanban.html\n";
        std::cout << "    reporte2_responsables.html\n";
        std::cout << "    reporte3_errores.html\n";
        std::cout << "    arbol.dot\n";
    } else {
        std::cout << "  Error al generar reportes.\n";
    }
}

void menuInteractivo() {
    std::cout << "\n";
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║   TaskScript Analyzer — Modo Consola     ║\n";
    std::cout << "║   LFP USAC 2026                          ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "  [1] Analizar archivo .task\n";
    std::cout << "  [2] Escribir código directamente\n";
    std::cout << "  [3] Ejecutar ejemplo de prueba rápida\n";
    std::cout << "  [0] Salir\n";
    std::cout << "\n";

    while (true) {
        std::cout << "Opción: ";
        int opcion;
        std::cin >> opcion;
        std::cin.ignore();

        if (opcion == 0) {
            std::cout << "Saliendo...\n";
            break;
        }

        if (opcion == 1) {
            std::cout << "Ruta del archivo .task: ";
            std::string ruta;
            std::getline(std::cin, ruta);

            std::ifstream f(ruta);
            if (!f.is_open()) {
                std::cout << "  Error: no se pudo abrir '" << ruta << "'\n";
                continue;
            }
            std::ostringstream ss;
            ss << f.rdbuf();
            analizar(ss.str(), "./reportes");
        }

        else if (opcion == 2) {
            std::cout << "Escribe el código TaskScript.\n";
            std::cout << "Termina con una línea que solo diga FIN\n\n";

            std::string fuente, linea;
            while (std::getline(std::cin, linea)) {
                if (linea == "FIN") break;
                fuente += linea + "\n";
            }
            analizar(fuente, "./reportes");
        }

        else if (opcion == 3) {
            std::cout << "\nEjecutando ejemplo integrado...\n";
            std::string ejemplo = R"(
TABLERO "Proyecto de Prueba" {
    COLUMNA "Por Hacer" {
        tarea: "Implementar Lexer" [prioridad: ALTA, responsable: "Ana",
            fecha_limite: 2026-05-01],
        tarea: "Implementar Parser" [prioridad: ALTA, responsable: "Luis",
            fecha_limite: 2026-05-08]
    };
    COLUMNA "Completado" {
        tarea: "Disenar gramatica" [prioridad: MEDIA, responsable: "Ana",
            fecha_limite: 2026-04-25]
    };
};
)";
            analizar(ejemplo, "./reportes");
        }

        else {
            std::cout << "  Opción no válida.\n";
        }

        std::cout << "\n";
        std::cout << "  [1] Analizar archivo .task\n";
        std::cout << "  [2] Escribir código directamente\n";
        std::cout << "  [3] Ejecutar ejemplo de prueba rápida\n";
        std::cout << "  [0] Salir\n\n";
    }
}

// ─────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────
int main(int argc, char* argv[]) {
    if (argc >= 2) {
        std::ifstream f(argv[1]);
        if (!f.is_open()) {
            std::cerr << "No se pudo abrir: " << argv[1] << "\n";
            return 1;
        }
        std::ostringstream ss;
        ss << f.rdbuf();
        std::string carpeta = (argc >= 3) ? argv[2] : "./reportes";
        analizar(ss.str(), carpeta);
    } else {
        menuInteractivo();
    }
    return 0;
}