#include "Reportes.h"
#include "Estadistica.h"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <map>

void reporteCursos(const std::vector<Curso>& cursos, const std::vector<Nota>& notas) {
    std::ofstream html("reporteCursos.html");
    html << "<html><body><h1>Estadísticas por Curso</h1><table border='1'>";
    html << "<tr><th>Curso</th><th>Estudiantes</th><th>Promedio</th><th>Máxima</th><th>Mínima</th><th>Mediana</th><th>Desv. Estándar</th></tr>";

    for (auto &c : cursos) {
        std::vector<double> calificaciones;
        for (auto &n : notas) {
            if (n.codigoCurso == c.codigo) calificaciones.push_back(n.nota);
        }
        if (!calificaciones.empty()) {
            double prom = promedio(calificaciones);
            double med = mediana(calificaciones);
            double desv = desviacionEstandar(calificaciones);
            double max = *max_element(calificaciones.begin(), calificaciones.end());
            double min = *min_element(calificaciones.begin(), calificaciones.end());

            html << "<tr><td>" << c.nombre << "</td><td>" << calificaciones.size()
                 << "</td><td>" << prom << "</td><td>" << max << "</td><td>" << min
                 << "</td><td>" << med << "</td><td>" << desv << "</td></tr>";
        }
    }
    html << "</table></body></html>";
}

void reporteEstudiantes(const std::vector<Estudiante>& estudiantes,
                        const std::vector<Nota>& notas,
                        const std::vector<Curso>& cursos) {
    std::ofstream html("reporteEstudiantes.html");
    html << "<html><body><h1>Rendimiento por Estudiante</h1><table border='1'>";
    html << "<tr><th>Carnet</th><th>Nombre</th><th>Carrera</th><th>Semestre</th><th>Promedio</th><th>Aprobados</th><th>Reprobados</th><th>Créditos</th></tr>";

    for (auto &e : estudiantes) {
        std::vector<double> calificaciones;
        int aprobados = 0, reprobados = 0, creditos = 0;

        for (auto &n : notas) {
            if (n.carnet == e.carnet) {
                calificaciones.push_back(n.nota);
                if (n.nota >= 61) aprobados++;
                else reprobados++;

                for (auto &c : cursos) {
                    if (c.codigo == n.codigoCurso) {
                        creditos += c.creditos;
                        break;
                    }
                }
            }
        }

        double prom = promedio(calificaciones);

        html << "<tr><td>" << e.carnet << "</td><td>" << e.nombre << " " << e.apellido
             << "</td><td>" << e.carrera << "</td><td>" << e.semestre
             << "</td><td>" << prom << "</td><td>" << aprobados
             << "</td><td>" << reprobados << "</td><td>" << creditos << "</td></tr>";
    }
    html << "</table></body></html>";
}


void reporteTop10(const std::vector<Estudiante>& estudiantes,
                  const std::vector<Nota>& notas) {
    struct Info {
        Estudiante e;
        double promedio;
    };
    std::vector<Info> lista;

    for (auto &e : estudiantes) {
        std::vector<double> calificaciones;
        for (auto &n : notas) if (n.carnet == e.carnet) calificaciones.push_back(n.nota);
        if (!calificaciones.empty()) lista.push_back({e, promedio(calificaciones)});
    }

    std::sort(lista.begin(), lista.end(), [](const Info& a, const Info& b){
        return a.promedio > b.promedio;
    });

    std::ofstream html("reporteTop10.html");
    html << "<html><body><h1>Top 10 Mejores Estudiantes</h1><table border='1'>";
    html << "<tr><th>Posición</th><th>Carnet</th><th>Nombre</th><th>Carrera</th><th>Semestre</th><th>Promedio</th></tr>";

    for (int i=0; i<10 && i<lista.size(); i++) {
        html << "<tr><td>" << (i+1) << "</td><td>" << lista[i].e.carnet
             << "</td><td>" << lista[i].e.nombre << " " << lista[i].e.apellido
             << "</td><td>" << lista[i].e.carrera << "</td><td>" << lista[i].e.semestre
             << "</td><td>" << lista[i].promedio << "</td></tr>";
    }
    html << "</table></body></html>";
}


void reporteReprobacion(const std::vector<Curso>& cursos,
                        const std::vector<Nota>& notas) {
    std::ofstream html("reporteReprobacion.html");
    html << "<html><body><h1>Cursos con Mayor Índice de Reprobación</h1><table border='1'>";
    html << "<tr><th>Curso</th><th>Total Estudiantes</th><th>Reprobados</th><th>Porcentaje</th></tr>";

    for (auto &c : cursos) {
        int total = 0, reprobados = 0;
        for (auto &n : notas) {
            if (n.codigoCurso == c.codigo) {
                total++;
                if (n.nota < 61) reprobados++;
            }
        }
        if (total > 0) {
            double porcentaje = (double)reprobados / total * 100.0;
            html << "<tr><td>" << c.nombre << "</td><td>" << total
                 << "</td><td>" << reprobados << "</td><td>" << porcentaje << "%</td></tr>";
        }
    }
    html << "</table></body></html>";
}


void reporteCarreras(const std::vector<Estudiante>& estudiantes,
                     const std::vector<Curso>& cursos,
                     const std::vector<Nota>& notas) {
    std::ofstream html("reporteCarreras.html");
    html << "<html><body><h1>Análisis por Carrera</h1><table border='1'>";
    html << "<tr><th>Carrera</th><th>Estudiantes</th><th>Promedio General</th></tr>";

    std::map<std::string, std::vector<double>> carreraNotas;

    for (auto &e : estudiantes) {
        for (auto &n : notas) {
            if (n.carnet == e.carnet) {
                carreraNotas[e.carrera].push_back(n.nota);
            }
        }
    }

    for (auto &par : carreraNotas) {
        double prom = promedio(par.second);
        html << "<tr><td>" << par.first << "</td><td>" << par.second.size()
             << "</td><td>" << prom << "</td></tr>";
    }

    html << "</table></body></html>";
}