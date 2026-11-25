#ifndef TURNO_CONTROLLER_H
#define TURNO_CONTROLLER_H

#include "../entities/Empleado.h"
#include "../entities/MallaTurnos.h"
#include "../entities/ReglaLaboral.h"
#include "../entities/Turno.h"
#include "../repository/TurnoRepository.h"
#include "GeneradorMalla.h"
#include "ValidadorTurnos.h"
#include <string>
#include <vector>

class TurnoController {
private:
    ValidadorTurnos validador;
    GeneradorMalla generador;
    TurnoRepository repo;

public:
    TurnoController() : generador(&validador) {}

    bool configurarRegla(int maxHoras, int minDescanso) {
        if (maxHoras <= 0 || maxHoras > 168 || minDescanso <= 0 || minDescanso > 48) {
            return false;
        }
        repo.guardarRegla(ReglaLaboral(maxHoras, minDescanso));
        return true;
    }

    MallaTurnos ejecutarGeneracionMalla(const std::string& periodo) {
        std::vector<Empleado> empleados = repo.obtenerEmpleados();
        ReglaLaboral regla = repo.obtenerRegla();
        MallaTurnos nuevaMalla = generador.generarProgramacion(empleados, regla, periodo);
        repo.guardarMalla(nuevaMalla);
        return nuevaMalla;
    }

    std::string validarAsignacionManual(const std::string& idEmp, int inicio, int fin) {
        if (inicio < 0 || fin <= inicio || fin > 168) {
            return "Error: Rango horario invalido (use horas de la semana entre 0 y 168, con fin > inicio).";
        }

        std::vector<Empleado> empleados = repo.obtenerEmpleados();
        const Empleado* empEncontrado = nullptr;
        for (const auto& emp : empleados) {
            if (emp.getId() == idEmp) {
                empEncontrado = &emp;
                break;
            }
        }

        if (empEncontrado == nullptr) {
            return "Error: No existe un empleado registrado con el ID '" + idEmp + "'.";
        }

        ReglaLaboral regla = repo.obtenerRegla();
        std::vector<Turno> turnosPrevios = repo.obtenerTurnosGuardados();
        Turno nuevoTurno("T-MANUAL", idEmp, inicio, fin);

        bool descansoOk = validador.verificarDescanso(turnosPrevios, nuevoTurno, regla);
        bool topeOk = validador.verificarTopeSemanal(*empEncontrado, nuevoTurno, regla);

        if (descansoOk && topeOk) {
            return "APROBADO: La asignacion cumple el descanso minimo (" +
                   std::to_string(regla.getMinDescansoHoras()) + "h) y el tope semanal (" +
                   std::to_string(regla.getMaxHorasSemanales()) + "h).";
        }
        if (!descansoOk) {
            return "RECHAZADO: Incumple el descanso minimo legal de " +
                   std::to_string(regla.getMinDescansoHoras()) + "h entre jornadas o se solapa con otro turno.";
        }
        return "RECHAZADO: Excede el tope maximo semanal de " +
               std::to_string(regla.getMaxHorasSemanales()) + "h (acumuladas: " +
               std::to_string(empEncontrado->obtenerHorasAcumuladas()) + "h + turno: " +
               std::to_string(nuevoTurno.getDuracion()) + "h).";
    }

    std::vector<Empleado> listarEmpleados() const {
        return repo.obtenerEmpleados();
    }
};

#endif