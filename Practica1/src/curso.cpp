#include "Curso.h"
#include <sstream>
#include <fstream>
#include <iostream>

std::vector<std::string> split(const std::string &s, char delimitador) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimitador)) {
        tokens.push_back(token);
    }
    return tokens;

}

Curso crearCurso(const std::string& linea) {
    Curso c;
    auto campos = split(linea, ',');
    c.codigo   = stoi(campos[0]);
    c.nombre   = campos[1];
    c.creditos = stoi(campos[2]);
    c.semestre = stoi(campos[3]);
    c.carrera  = campos[4];
    return c;
}
vector<Curso> cargarCursos(const string& ruta) {
    vector<Curso> cursos;
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << ruta << endl;
        return cursos;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            cursos.push_back(crearCurso(linea));
        }
    }

    archivo.close();
    return cursos;
}
