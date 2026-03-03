#ifndef NOTA_H
#define NOTA_H

#include <string>
#include <vector>
using namespace std;

struct Nota {
    int carnet;
    int codigoCurso;
    double nota;
    string ciclo;
    int anio;
};

Nota crearNota(const string& linea);
vector<Nota> cargarNotas(const string& ruta);

#endif