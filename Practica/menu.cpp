#include <iostream> // Biblioteca necesaria
using namespace std;
#include "Estudiante.h"

int main() {
    int opcion;
    vector<Estudiante> estudiante;
    cout << "SISTEMA DE ANALISIS ACADEMICO" << endl; 
    cout << "1. Cargar archivo de estudiante" << endl; 
    cout << "2. Cargar archivo de notas " << endl; 
    cout << "3. Cargar archivo de estudiante" << endl;
    cout << "4. Generar Reporte: Estadisticas por Curso" << endl;
    cout << "5. Generar Reporte: Rendimiento por Estudiante" << endl;
    cout << "6. Generar Reporte: Top 10 Mejores Estudiantes" << endl;
    cout << "7. Generar Reporte: Cursos con Mayor Reprobacion" << endl;
    cout << "8. Generar Reporte: Analisis por Carrera" << endl;
    cout << "9. Salir" << endl;
    cout << "Seleccione una opcion" << endl;
    cin >> opcion;
    if(opcion<=9){
        switch (opcion)
        {
            case 1: 
            estudiante = cargarEstudiantes("estudiante.lfp");
            cout << "Estudiantes cargados " << estudiante.size() << endl; 
            break;
            case 2: cout << "Cargando estudiantes ...." << endl; break;
            case 3: cout << "Cargando estudiantes ...." << endl;  break;
            case 9: exit(0);
        default:
            break;
        }
    }else{
        cout << "error de opcion";
    }
    system("pause");
    return 0;
}
