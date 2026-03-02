#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>

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
    std::string nombre;
    int creditos;
    int semestre;
    std::string carrera;
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

int main() {
    vector<Estudiante> lista = cargarEstudiantes("estudiante.lfp");

    for (auto &e : lista) {
        cout << e.carnet << " - " << e.nombre << " " << e.apellido
             << " (" << e.carrera << ", Semestre " << e.semestre << ")" << endl;
    }
    vector<Curso> lista2 = cargarCursos("cursos.lfp");
    for (auto &e : lista2) {
        cout << e.codigo << " - " << e.nombre << " " << e.creditos
             << " (" << e.carrera << ", Semestre " << e.semestre << ")" << endl;
    }
    system("pause");
    return 0;
}
/*int main(){
    ifstream archivo("datos.lfp");
    while(getline(archivo,linea)){
        texto = texto+linea+"\n";
    }
    stringstream sn(texto);
    while(getline(sn,item,',')){
        tokens.push_back(item);
    }
    for(const string& t : tokens){
        cout << t << endl;
    }
    system("pause");
}*/;