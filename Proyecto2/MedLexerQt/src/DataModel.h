#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <string>

struct Paciente {
    std::string nombre;
    int edad;
    std::string sangre;
    int habitacion;
};

struct Medico {
    std::string nombre;
    std::string especialidad;
    std::string codigo;
};

struct Cita {
    std::string paciente;
    std::string medico;
    std::string fecha;
    std::string hora;
};

struct Diagnostico {
    std::string paciente;
    std::string condicion;
    std::string medicamento;
    std::string dosis;
};

#endif