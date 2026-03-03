#include "Estudiante.h"
#include <sstream>
#include <fstream>
#include <iostream>

std::vector<std::string> split2(const std::string &s, char delimitador) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimitador)) {
        tokens.push_back(token);
    }
    return tokens;

}

Estudiante crearEstudiante(const std::string& linea) {
    Estudiante c;
    auto campos = split2(linea, ',');
    c.carnet   = stoi(campos[0]);
    c.nombre   = campos[1];
    c.apellido = campos[2];
    c.carrera = campos[3];
    c.semestre  = stoi(campos[4]);
    return c;
}

std::vector<Estudiante> cargarEstudiantes(const std::string& ruta) {
    std::vector<Estudiante> estudiantes;
    std::ifstream archivo(ruta);

    if (!archivo.is_open()) {
        std::cerr << "Error: no se pudo abrir el archivo " << ruta << std::endl;
        return estudiantes;
    }

    std::string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            estudiantes.push_back(crearEstudiante(linea));
        }
    }

    archivo.close();
    return estudiantes;
}