#ifndef ESTUDIANTE_H
#define ESTUDIANTE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

string linea;
string texto; 
string item;
vector<string> tokens;

struct Estudiante {
    int carnet;
    string nombre;
    string apellido;
    string carrera;
    int semestre;
};
vector<Estudiante> cargarEstudiantes(string ruta) {
    vector<Estudiante> estudiantes;
    ifstream archivo(ruta);
    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Estudiante e;
        getline(ss, campo, ','); e.carnet = stoi(campo);
        getline(ss, e.nombre, ',');
        getline(ss, e.apellido, ',');
        getline(ss, e.carrera, ',');
        getline(ss, campo, ','); e.semestre = stoi(campo);
        estudiantes.push_back(e);
    }
    return estudiantes;
}
struct Curso {
    int codigo;
    string nombre;
    int creditos;
    int semestre;
    string carrera;
};
vector<Curso> cargarCursos(string ruta) {
    vector<Curso> Cursos;
    ifstream archivo(ruta);
    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Curso e;
        getline(ss, campo, ','); e.codigo = stoi(campo);
        getline(ss, e.nombre, ',');
        getline(ss, campo, ','); e.creditos = stoi(campo);
        getline(ss, campo, ','); e.semestre = stoi(campo);
        getline(ss, e.carrera, ',');
        Cursos.push_back(e);
    }
    return Cursos;
}
struct Nota{
    int carnet;
    string codigo;
    int nota;
    string ciclo;
    int anio;
};
vector<Nota> cargarNotas(string ruta) {
    vector<Nota> Notas;
    ifstream archivo(ruta);
    string linea;
    while (getline(archivo, linea)) {
        stringstream ss(linea);
        string campo;
        Nota e;
        getline(ss, campo, ','); e.carnet = stoi(campo);
        getline(ss, e.codigo, ',');
        getline(ss, campo, ','); e.nota = stoi(campo);
        getline(ss, e.ciclo, ',');
        getline(ss, campo, ','); e.anio = stoi(campo);
        Notas.push_back(e);
    }
    return Notas;
}

#endif