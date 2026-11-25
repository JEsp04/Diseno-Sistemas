#ifndef MALLA_TURNOS_H
#define MALLA_TURNOS_H

#include "Turno.h"
#include <string>
#include <vector>

class MallaTurnos {
private:
    std::string periodo;
    std::vector<Turno> turnos;

public:
    explicit MallaTurnos(std::string periodo = "Sin generar")
        : periodo(std::move(periodo)) {}

    void agregarTurno(const Turno& turno) {
        turnos.push_back(turno);
    }

    std::vector<Turno> obtenerTurnos() const {
        return turnos;
    }

    std::string getPeriodo() const { return periodo; }
};

#endif