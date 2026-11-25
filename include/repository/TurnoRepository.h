#ifndef TURNO_REPOSITORY_H
#define TURNO_REPOSITORY_H

#include "../entities/Empleado.h"
#include "../entities/MallaTurnos.h"
#include "../entities/ReglaLaboral.h"
#include <vector>

class TurnoRepository {
private:
    std::vector<Empleado> listaEmpleados;
    MallaTurnos mallaActual;
    ReglaLaboral reglaActiva;

public:
    TurnoRepository() : mallaActual("Sin generar"), reglaActiva(47, 12) {
        // Plantilla operativa inicial para pruebas del Sprint 1
        listaEmpleados.emplace_back("EMP-01", "Laura Restrepo", "Operador 24/7");
        listaEmpleados.emplace_back("EMP-02", "Carlos Gomez", "Operador 24/7");
        listaEmpleados.emplace_back("EMP-03", "Sofia Martinez", "Operador 24/7");
        listaEmpleados.emplace_back("EMP-04", "Andres Ramirez", "Operador 24/7");
        listaEmpleados.emplace_back("EMP-05", "Valentina Rojas", "Operador 24/7");
    }

    void guardarRegla(const ReglaLaboral& regla) {
        reglaActiva = regla;
    }

    ReglaLaboral obtenerRegla() const {
        return reglaActiva;
    }

    std::vector<Empleado> obtenerEmpleados() const {
        return listaEmpleados;
    }

    void guardarMalla(const MallaTurnos& malla) {
        mallaActual = malla;
        // Sincroniza horas acumuladas de cada empleado segun la malla guardada
        for (auto& emp : listaEmpleados) {
            emp.reiniciarHoras();
        }
        for (const auto& turno : mallaActual.obtenerTurnos()) {
            for (auto& emp : listaEmpleados) {
                if (emp.getId() == turno.getIdEmpleado()) {
                    emp.sumarHoras(turno.getDuracion());
                    break;
                }
            }
        }
    }

    std::vector<Turno> obtenerTurnosGuardados() const {
        return mallaActual.obtenerTurnos();
    }
};

#endif