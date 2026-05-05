#pragma once
#include <string>
#include <vector>

enum class TipoError { LEXICO, SINTACTICO };


struct ErrorAnalisis {
    int         numero;
    std::string lexema;
    TipoError   tipo;
    std::string descripcion;
    int         linea;
    int         columna;

    std::string tipoStr() const {
        return tipo == TipoError::LEXICO ? "Lexico" : "Sintactico";
    }
};


class ErrorManager {
public:
    void agregarError(const std::string& lexema,
                      TipoError tipo,
                      const std::string& descripcion,
                      int linea, int columna)
    {
        errores.push_back({
            (int)errores.size() + 1,
            lexema, tipo, descripcion, linea, columna
        });
    }

    bool hayErrores() const { return !errores.empty(); }

    const std::vector<ErrorAnalisis>& obtenerErrores() const {
        return errores;
    }

    void limpiar() { errores.clear(); }

private:
    std::vector<ErrorAnalisis> errores;
};
