#include "Reporte.h"
#include <fstream>
#include <set>
#include <vector>
void ReportGenerator::generarTodo(std::vector<Paciente>& pacientes,
                                  std::vector<Medico>& medicos,
                                  std::vector<Cita>& citas,
                                  std::vector<Diagnostico>& diagnosticos) {

    generatePacientes(pacientes, diagnosticos);
    generateMedicos(medicos, citas);
    generateCitas(citas, medicos);
    generateEstadisticas(pacientes, medicos, citas, diagnosticos);
    generateGraphviz(pacientes, medicos, citas, diagnosticos);
}

//////////////////////////////////////////////////////////
// 📊 REPORTE 1 — PACIENTES
//////////////////////////////////////////////////////////
void ReportGenerator::generatePacientes(std::vector<Paciente>& pacientes,
                                        std::vector<Diagnostico>& diagnosticos) {

    std::ofstream file("../reports/pacientes.html");

    file << "<html><body><h1>Historial de Pacientes</h1>";
    file << "<table border='1'>";

    for (auto &p : pacientes) {
        file << "<tr>";
        file << "<td>" << p.nombre << "</td>";
        file << "<td>" << p.edad << "</td>";
        file << "<td>" << p.sangre << "</td>";

        bool tieneDiag = false;

        for (auto &d : diagnosticos) {
            if (d.paciente == p.nombre) {
                file << "<td>" << d.condicion << "</td>";
                file << "<td>" << d.medicamento << " / " << d.dosis << "</td>";
                file << "<td style='color:green'>ACTIVO</td>";
                tieneDiag = true;
            }
        }

        if (!tieneDiag) {
            file << "<td>Sin diagnóstico</td><td>-</td>";
            file << "<td style='color:orange'>SIN DIAG.</td>";
        }

        file << "</tr>";
    }

    file << "</table></body></html>";
}

//////////////////////////////////////////////////////////
// 👨‍⚕️ REPORTE 2 — MÉDICOS
//////////////////////////////////////////////////////////
void ReportGenerator::generateMedicos(std::vector<Medico>& medicos,
                                      std::vector<Cita>& citas) {

    std::ofstream file("../reports/medicos.html");

    file << "<html><body><h1>Carga de Médicos</h1><table border='1'>";

    for (auto &m : medicos) {

        int citasCount = 0;
        std::set<std::string> pacientesUnicos;

        for (auto &c : citas) {
            if (c.medico == m.nombre) {
                citasCount++;
                pacientesUnicos.insert(c.paciente);
            }
        }

        std::string nivel = "BAJA";
        std::string color = "blue";

        if (citasCount >= 4) { nivel = "NORMAL"; color = "green"; }
        if (citasCount >= 7) { nivel = "ALTA"; color = "orange"; }
        if (citasCount >= 9) { nivel = "SATURADA"; color = "red"; }

        file << "<tr>";
        file << "<td>" << m.nombre << "</td>";
        file << "<td>" << m.codigo << "</td>";
        file << "<td>" << m.especialidad << "</td>";
        file << "<td>" << citasCount << "</td>";
        file << "<td>" << pacientesUnicos.size() << "</td>";
        file << "<td style='color:" << color << "'>" << nivel << "</td>";
        file << "</tr>";
    }

    file << "</table></body></html>";
}

//////////////////////////////////////////////////////////
// 📅 REPORTE 3 — CITAS + CONFLICTOS
//////////////////////////////////////////////////////////
void ReportGenerator::generateCitas(std::vector<Cita>& citas,
                                    std::vector<Medico>& medicos) {

    std::ofstream file("../reports/citas.html");

    file << "<html><body><h1>Agenda de Citas</h1><table border='1'>";

    for (int i = 0; i < citas.size(); i++) {

        bool conflicto = false;

        for (int j = 0; j < citas.size(); j++) {
            if (i != j &&
                citas[i].medico == citas[j].medico &&
                citas[i].fecha == citas[j].fecha &&
                citas[i].hora == citas[j].hora) {
                conflicto = true;
            }
        }

        std::string color = conflicto ? "red" : "green";
        std::string estado = conflicto ? "CONFLICTO" : "CONFIRMADA";

        file << "<tr style='background:" << color << "'>";
        file << "<td>" << citas[i].fecha << "</td>";
        file << "<td>" << citas[i].hora << "</td>";
        file << "<td>" << citas[i].paciente << "</td>";
        file << "<td>" << citas[i].medico << "</td>";
        file << "<td>" << estado << "</td>";
        file << "</tr>";
    }

    file << "</table></body></html>";
}

//////////////////////////////////////////////////////////
// 📈 REPORTE 4 — ESTADÍSTICAS
//////////////////////////////////////////////////////////
void ReportGenerator::generateEstadisticas(std::vector<Paciente>& pacientes,
                                           std::vector<Medico>& medicos,
                                           std::vector<Cita>& citas,
                                           std::vector<Diagnostico>& diagnosticos) {

    std::ofstream file("../reports/estadisticas.html");

    int totalPacientes = pacientes.size();
    int totalMedicos = medicos.size();
    int totalCitas = citas.size();

    int conDiagnostico = diagnosticos.size();

    float porcentaje = totalPacientes > 0 ?
        (conDiagnostico * 100.0f / totalPacientes) : 0;

    file << "<html><body><h1>Estadísticas</h1>";

    file << "<p>Total pacientes: " << totalPacientes << "</p>";
    file << "<p>Total médicos: " << totalMedicos << "</p>";
    file << "<p>Total citas: " << totalCitas << "</p>";
    file << "<p>Pacientes con diagnóstico: " << porcentaje << "%</p>";

    file << "</body></html>";
}

//////////////////////////////////////////////////////////
// 🌐 GRAPHVIZ
//////////////////////////////////////////////////////////
void ReportGenerator::generateGraphviz(std::vector<Paciente>& pacientes,
                                       std::vector<Medico>& medicos,
                                       std::vector<Cita>& citas,
                                       std::vector<Diagnostico>& diagnosticos) {

    std::ofstream file("../graph/hospital.dot");

    file << "digraph Hospital {\n";

    for (auto &c : citas) {
        file << "\"" << c.paciente << "\" -> \"" << c.medico
             << "\" [label=\"" << c.fecha << " " << c.hora << "\"];\n";
    }

    for (auto &d : diagnosticos) {
        file << "\"" << d.condicion << "\" -> \"" << d.paciente << "\";\n";
    }

    file << "}";
}