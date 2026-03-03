#ifndef CURSO_H
#define CURSO_H

#include <string>
#include <vector>
using namespace std;

struct Curso {
    int codigo;      
    string nombre;   
    int creditos;   
    int semestre;    
    string carrera;   
};

Curso crearCurso(const string& linea);
vector<Curso> cargarCursos(const string& ruta);

#endif