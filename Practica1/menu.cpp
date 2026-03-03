#include <iostream> // Biblioteca necesaria
#include "Estudiante.h"
#include "Nota.h"
#include "Curso.h"
#include "reportes.h"
using namespace std;

int main()
{
    int opcion = 0;
    vector<Estudiante> estudiante;
    vector<Nota> nota;
    vector<Curso> curso;
    while (true)
    {
        cout << "SISTEMA DE ANALISIS ACADEMICO" << endl;
        cout << "1. Cargar archivo de estudiante" << endl;
        cout << "2. Cargar archivo de notas " << endl;
        cout << "3. Cargar archivo de curso" << endl;
        cout << "4. Generar Reporte: Estadisticas por Curso" << endl;
        cout << "5. Generar Reporte: Rendimiento por Estudiante" << endl;
        cout << "6. Generar Reporte: Top 10 Mejores Estudiantes" << endl;
        cout << "7. Generar Reporte: Cursos con Mayor Reprobacion" << endl;
        cout << "8. Generar Reporte: Analisis por Carrera" << endl;
        cout << "9. Salir" << endl;
        cout << "Seleccione una opcion" << endl;
        cin >> opcion;
        if (opcion <= 9)
        {
            switch (opcion)
            {
            case 1:
                estudiante = cargarEstudiantes("estudiante.lfp");
                cout << "Estudiantes cargados " << estudiante.size() << endl;
                break;
            case 2:
                nota = cargarNotas("notas.lfp");
                cout << "Notas cargados corerectamente " << endl;
                break;
            case 3:
                curso = cargarCursos("cursos.lfp");
                cout << "Notas cargados corerectamente " << endl;
                break;
            case 4:
                reporteCursos(curso, nota);
                cout << "Reporte creado exitosamente" << endl;
                break;
            case 5:
                reporteEstudiantes(estudiante, nota, curso);
                cout << "Reporte creado exitosamente" << endl;
                break;
            case 6:
                reporteTop10(estudiante, nota);
                cout << "Reporte creado exitosamente" << endl;
                break;
            case 7:
                reporteReprobacion(curso, nota);
                cout << "Reporte creado exitosamente" << endl;
                break;
            case 8:
                reporteCarreras(estudiante,curso,nota);
                cout << "Reporte creado exitosamente" << endl;
                break;
            case 9:
                exit(0);
            default:
                break;
            }
        } else{
            cout << "Error de opcion" << endl;
        }
    }
}