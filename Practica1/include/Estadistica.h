#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include <vector>


double promedio(const std::vector<double>& datos);
double mediana(std::vector<double> datos);
double desviacionEstandar(const std::vector<double>& datos);
double percentil(std::vector<double> datos, double p);

#endif