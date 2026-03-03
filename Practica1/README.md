# 📘 Guía de Uso del Sistema de Análisis Académico

Este documento explica cómo **compilar, ejecutar y usar** el sistema desde la consola.  
Incluye ejemplos prácticos y capturas de pantalla simuladas del menú y reportes.

---

## 🔹 1. Compilación del programa

Desde la carpeta principal del proyecto (`Practica/`):

```bash
g++ menu.cpp src/*.cpp -Iinclude -o practica1

EJECUCION DEL PROGRAMA

./practica1

MENU DE NAVEGACION:
===== SISTEMA DE ANALISIS ACADEMICO =====
1. Cargar archivo de estudiantes
2. Cargar archivo de notas
3. Cargar archivo de cursos
4. Generar Reporte: Estadisticas por Curso
5. Generar Reporte: Rendimiento por Estudiante
6. Generar Reporte: Top 10 Mejores Estudiantes
7. Generar Reporte: Cursos con Mayor Reprobacion
8. Generar Reporte: Analisis por Carrera
9. Salir
Seleccione una opcion:

EJEMPLO CARGA DE ARCHIVOS:
Seleccione una opcion: 1
Estudiantes cargados: 120

Seleccione una opcion: 2
Notas cargadas: 450

Seleccione una opcion: 3
Cursos cargados: 35

REPORTES EJEMPLO:
Seleccione una opcion: 
Reporte de cursos generado en reporteCursos.html