#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include "Token.h"
#include "DataModel.h"

class Parser {
public:
    Parser(std::vector<Token> tokens);

    void parse();

    std::vector<Paciente> pacientes;
    std::vector<Medico> medicos;
    std::vector<Cita> citas;
    std::vector<Diagnostico> diagnosticos;

private:
    std::vector<Token> tokens;
    int pos;

    Token current();
    void advance();

    void parsePacientes();
    void parseMedicos();
    void parseCitas();
    void parseDiagnosticos();
};

#endif