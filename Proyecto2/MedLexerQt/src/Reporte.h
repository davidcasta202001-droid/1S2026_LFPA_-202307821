#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <vector>
#include "DataModel.h"

class ReportGenerator {
public:
    void generarTodo(std::vector<Paciente>& pacientes,
                     std::vector<Medico>& medicos,
                     std::vector<Cita>& citas,
                     std::vector<Diagnostico>& diagnosticos);

private:
    void generatePacientes(std::vector<Paciente>& pacientes,
                           std::vector<Diagnostico>& diagnosticos);

    void generateMedicos(std::vector<Medico>& medicos,
                         std::vector<Cita>& citas);

    void generateCitas(std::vector<Cita>& citas,
                       std::vector<Medico>& medicos);

    void generateEstadisticas(std::vector<Paciente>& pacientes,
                              std::vector<Medico>& medicos,
                              std::vector<Cita>& citas,
                              std::vector<Diagnostico>& diagnosticos);

    void generateGraphviz(std::vector<Paciente>& pacientes,
                          std::vector<Medico>& medicos,
                          std::vector<Cita>& citas,
                          std::vector<Diagnostico>& diagnosticos);
};

#endif
