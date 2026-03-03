#include "Nota.h"
#include <sstream>
#include <fstream>
#include <iostream>

vector<std::string> split3(const string &s, char delimitador) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimitador)) {
        tokens.push_back(token);
    }
    return tokens;

}

Nota crearNota(const string& linea) {
    Nota c;
    auto campos = split3(linea, ',');
    c.carnet   = stoi(campos[0]);
    c.codigoCurso   = stoi(campos[1]);
    c.nota = stod(campos[2]);
    c.ciclo = campos[3];
    c.anio  = stoi(campos[4]);
    return c;
}

vector<Nota> cargarNotas(const string& ruta) {
    vector<Nota> notas;
    ifstream archivo(ruta);

    if (!archivo.is_open()) {
        cerr << "Error: no se pudo abrir el archivo " << ruta << endl;
        return notas;
    }

    string linea;
    while (getline(archivo, linea)) {
        if (!linea.empty()) {
            notas.push_back(crearNota(linea));
        }
    }

    archivo.close();
    return notas;
}