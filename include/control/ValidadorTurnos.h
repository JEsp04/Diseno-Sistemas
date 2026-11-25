#ifndef VALIDADOR_TURNOS_H
#define VALIDADOR_TURNOS_H

#include "../entities/Empleado.h"
#include "../entities/ReglaLaboral.h"
#include "../entities/Turno.h"
#include <vector>

class ValidadorTurnos {
public:
    bool verificarDescanso(const std::vector<Turno>& turnosPrevios,
                           const Turno& nuevoTurno,
                           const ReglaLaboral& regla) const {
        for (const auto& previo : turnosPrevios) {
            if (previo.getIdEmpleado() != nuevoTurno.getIdEmpleado()) {
                continue;
            }
            // Rechaza solapamiento directo
            if (nuevoTurno.getHoraInicio() < previo.getHoraFin() &&
                previo.getHoraInicio() < nuevoTurno.getHoraFin()) {
                return false;
            }
            // Verifica descanso minimo despues del turno previo
            if (nuevoTurno.getHoraInicio() >= previo.getHoraFin()) {
                if ((nuevoTurno.getHoraInicio() - previo.getHoraFin()) < regla.getMinDescansoHoras()) {
                    return false;
                }
            }
            // Verifica descanso minimo antes de un turno posterior
            if (previo.getHoraInicio() >= nuevoTurno.getHoraFin()) {
                if ((previo.getHoraInicio() - nuevoTurno.getHoraFin()) < regla.getMinDescansoHoras()) {
                    return false;
                }
            }
        }
        return true;
    }

    bool verificarTopeSemanal(const Empleado& emp,
                              const Turno& nuevoTurno,
                              const ReglaLaboral& regla) const {
        return (emp.obtenerHorasAcumuladas() + nuevoTurno.getDuracion()) <= regla.getMaxHorasSemanales();
    }
};

#endif