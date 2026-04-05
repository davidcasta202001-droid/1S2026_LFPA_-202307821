#include "Parser.h"

Parser::Parser(std::vector<Token> tokens)
    : tokens(tokens), pos(0) {}

Token Parser::current() {
    if (pos < tokens.size())
        return tokens[pos];
    return Token(TokenType::END, "", 0, 0);
}

void Parser::advance() {
    if (pos < tokens.size()) pos++;
}

void Parser::parse() {
    while (pos < tokens.size()) {

        if (current().lexeme == "PACIENTES") {
            parsePacientes();
        }
        else if (current().lexeme == "MEDICOS") {
            parseMedicos();
        }
        else if (current().lexeme == "CITAS") {
            parseCitas();
        }
        else if (current().lexeme == "DIAGNOSTICOS") {
            parseDiagnosticos();
        }

        advance();
    }
}
void Parser::parsePacientes() {

    advance(); // PACIENTES

    while (pos < tokens.size() && current().lexeme != "]") {

        Paciente p;

        advance(); // nombre
        p.nombre = current().lexeme;

        advance(); // edad
        try {
            p.edad = std::stoi(current().lexeme);
        } catch (...) {
            p.edad = 0;
        }

        advance(); // sangre
        p.sangre = current().lexeme;

        advance(); // habitacion
        try {
            p.habitacion = std::stoi(current().lexeme);
        } catch (...) {
            p.habitacion = 0;
        }

        pacientes.push_back(p);

        advance();
    }
}
void Parser::parseMedicos() {

    advance();

    while (pos < tokens.size() && current().lexeme != "]") {

        Medico m;

        advance();
        m.nombre = current().lexeme;

        advance();
        m.especialidad = current().lexeme;

        advance();
        m.codigo = current().lexeme;

        medicos.push_back(m);

        advance();
    }
}
void Parser::parseCitas() {

    advance();

    while (pos < tokens.size() && current().lexeme != "]") {

        Cita c;

        advance();
        c.paciente = current().lexeme;

        advance();
        c.medico = current().lexeme;

        advance();
        c.fecha = current().lexeme;

        advance();
        c.hora = current().lexeme;

        citas.push_back(c);

        advance();
    }
}
void Parser::parseDiagnosticos() {

    advance();

    while (pos < tokens.size() && current().lexeme != "]") {

        Diagnostico d;

        advance();
        d.paciente = current().lexeme;

        advance();
        d.condicion = current().lexeme;

        advance();
        d.medicamento = current().lexeme;

        advance();
        d.dosis = current().lexeme;

        diagnosticos.push_back(d);

        advance();
    }
}