#ifndef REPORTES_H
#define REPORTES_H

#include <vector>
#include "Estudiante.h"
#include "Curso.h"
#include "Nota.h"


void reporteCursos(const std::vector<Curso>& cursos, const std::vector<Nota>& notas);
void reporteEstudiantes(const std::vector<Estudiante>& estudiantes,
                        const std::vector<Nota>& notas,
                        const std::vector<Curso>& cursos);
void reporteTop10(const std::vector<Estudiante>& estudiantes,
                  const std::vector<Nota>& notas);

void reporteReprobacion(const std::vector<Curso>& cursos,
                        const std::vector<Nota>& notas);

void reporteCarreras(const std::vector<Estudiante>& estudiantes,
                     const std::vector<Curso>& cursos,
                     const std::vector<Nota>& notas);

#endif