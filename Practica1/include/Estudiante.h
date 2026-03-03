#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include <string>
#include <vector>

struct Estudiante {
    int carnet;
    std::string nombre;
    std::string apellido;
    std::string carrera;
    int semestre;
};

Estudiante crearEstudiante(const std::string& linea);
std::vector<Estudiante> cargarEstudiantes(const std::string& ruta);

#endif