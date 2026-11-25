#ifndef GENERADOR_MALLA_H
#define GENERADOR_MALLA_H

#include "../entities/Empleado.h"
#include "../entities/MallaTurnos.h"
#include "../entities/ReglaLaboral.h"
#include "../entities/Turno.h"
#include "ValidadorTurnos.h"
#include <algorithm>
#include <string>
#include <vector>

class GeneradorMalla {
private:
    ValidadorTurnos* validador; // Agregacion: ciclo de vida independiente

public:
    explicit GeneradorMalla(ValidadorTurnos* validador) : validador(validador) {}

    MallaTurnos generarProgramacion(std::vector<Empleado> empleados,
                                    const ReglaLaboral& regla,
                                    const std::string& periodo) const {
        MallaTurnos malla(periodo);
        for (auto& emp : empleados) {
            emp.reiniciarHoras();
        }

        // 7 dias x 3 franjas de 8h (Manana: 6-14, Tarde: 14-22, Noche: 22-30)
        const int iniciosFranja[3] = {6, 14, 22};
        int consecutivo = 1;

        for (int dia = 0; dia < 7; ++dia) {
            for (int f = 0; f < 3; ++f) {
                int inicio = dia * 24 + iniciosFranja[f];
                int fin = inicio + 8;

                // Ordena por menor carga horaria para maximizar equidad
                std::sort(empleados.begin(), empleados.end(),
                          [](const Empleado& a, const Empleado& b) {
                              return a.obtenerHorasAcumuladas() < b.obtenerHorasAcumuladas();
                          });

                bool asignado = false;
                for (auto& emp : empleados) {
                    std::string idTurno = "T-" + std::to_string(consecutivo);
                    Turno candidato(idTurno, emp.getId(), inicio, fin);

                    bool descansoOk = validador->verificarDescanso(malla.obtenerTurnos(), candidato, regla);
                    bool topeOk = validador->verificarTopeSemanal(emp, candidato, regla);

                    if (descansoOk && topeOk) {
                        malla.agregarTurno(candidato);
                        emp.sumarHoras(candidato.getDuracion());
                        consecutivo++;
                        asignado = true;
                        break;
                    }
                }

                // Si ningun empleado cumple restricciones, marca franja descubierta
                if (!asignado) {
                    std::string idTurno = "T-" + std::to_string(consecutivo++);
                    malla.agregarTurno(Turno(idTurno, "SIN_COBERTURA", inicio, fin));
                }
            }
        }
        return malla;
    }
};

#endif