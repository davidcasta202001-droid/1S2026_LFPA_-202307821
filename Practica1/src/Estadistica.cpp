#include "Estadistica.h"
#include <algorithm>
#include <cmath>

double promedio(const std::vector<double>& datos) {
    if (datos.empty()) return 0.0;
    double suma = 0;
    for (double x : datos) suma += x;
    return suma / datos.size();
}

double mediana(std::vector<double> datos) {
    if (datos.empty()) return 0.0;
    std::sort(datos.begin(), datos.end());
    int n = datos.size();
    if (n % 2 == 0) {
        return (datos[n/2 - 1] + datos[n/2]) / 2.0;
    } else {
        return datos[n/2];
    }
}

double desviacionEstandar(const std::vector<double>& datos) {
    if (datos.empty()) return 0.0;
    double m = promedio(datos);
    double suma = 0;
    for (double x : datos) suma += std::pow(x - m, 2);
    return std::sqrt(suma / datos.size());
}

double percentil(std::vector<double> datos, double p) {
    if (datos.empty()) return 0.0;
    std::sort(datos.begin(), datos.end());
    int n = datos.size();
    double k = (p/100.0) * (n - 1);
    int i = std::floor(k);
    double frac = k - i;
    if (i+1 < n)
        return datos[i] + frac * (datos[i+1] - datos[i]);
    else
        return datos[i];
}