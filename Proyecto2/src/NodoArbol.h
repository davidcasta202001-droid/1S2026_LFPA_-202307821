#pragma once
#include <string>
#include <vector>
#include <memory>


struct NodoArbol {
    std::string etiqueta;
    bool        esTerminal;
    std::vector<std::shared_ptr<NodoArbol>> hijos;

    NodoArbol(const std::string& et, bool terminal = false)
        : etiqueta(et), esTerminal(terminal) {}

    void agregarHijo(std::shared_ptr<NodoArbol> hijo) {
        hijos.push_back(std::move(hijo));
    }
};

using NodoPtr = std::shared_ptr<NodoArbol>;

inline NodoPtr nodoNoTerminal(const std::string& nombre) {
    return std::make_shared<NodoArbol>(nombre, false);
}

inline NodoPtr nodoTerminal(const std::string& lexema) {
    return std::make_shared<NodoArbol>(lexema, true);
}
