#ifndef CONSOLA_SAT_VIEW_H
#define CONSOLA_SAT_VIEW_H

#include "../control/TurnoController.h"
#include "../entities/MallaTurnos.h"
#include "../entities/Supervisor.h"
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

class ConsolaSATView {
private:
    TurnoController controlador;
    Supervisor supervisorActivo;

    void limpiarEntrada() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string formatearHoraSemana(int horaAbsoluta) const {
        const std::string dias[7] = {"Lun", "Mar", "Mie", "Jue", "Vie", "Sab", "Dom"};
        int dia = (horaAbsoluta / 24) % 7;
        int hora = horaAbsoluta % 24;
        std::string hStr = (hora < 10 ? "0" : "") + std::to_string(hora) + ":00";
        return dias[dia] + " " + hStr;
    }

public:
    ConsolaSATView() : supervisorActivo("SUP-01", "Jhojan Espinosa", "Operaciones 24/7") {}

    void mostrarMenuPrincipal() {
        int opcion = -1;
        while (opcion != 0) {
            std::cout << "\n====================================================\n";
            std::cout << "  SISTEMA DE ASIGNACION DE TURNOS (SAT) - SPRINT 1\n";
            std::cout << "  Sesion: " << supervisorActivo.obtenerInfo() << "\n";
            std::cout << "====================================================\n";
            std::cout << "1. Configurar regla laboral (CU-01)\n";
            std::cout << "2. Generar malla automatica semanal (CU-02)\n";
            std::cout << "3. Probar validacion manual de turno\n";
            std::cout << "4. Listar empleados y horas acumuladas\n";
            std::cout << "0. Salir\n";
            std::cout << "Seleccione una opcion: ";

            if (!(std::cin >> opcion)) {
                limpiarEntrada();
                std::cout << "[!] Entrada invalida. Ingrese un numero del menu.\n";
                continue;
            }
            limpiarEntrada();

            switch (opcion) {
                case 1: capturarReglaLaboral(); break;
                case 2: solicitarGeneracionMalla(); break;
                case 3: probarValidacionManual(); break;
                case 4: mostrarEmpleados(); break;
                case 0: std::cout << "Cerrando sesion del SAT...\n"; break;
                default: std::cout << "[!] Opcion fuera de rango.\n"; break;
            }
        }
    }

    void capturarReglaLaboral() {
        int maxHoras = 0;
        int minDescanso = 0;
        std::cout << "\n--- Parametrizar Regla Laboral ---\n";
        std::cout << "Horas maximas semanales (ej. 47): ";
        if (!(std::cin >> maxHoras)) {
            limpiarEntrada();
            std::cout << "[ERROR] Debe ingresar un valor numerico valido.\n";
            return;
        }
        std::cout << "Descanso minimo entre jornadas en horas (ej. 12): ";
        if (!(std::cin >> minDescanso)) {
            limpiarEntrada();
            std::cout << "[ERROR] Debe ingresar un valor numerico valido.\n";
            return;
        }
        limpiarEntrada();

        if (controlador.configurarRegla(maxHoras, minDescanso)) {
            std::cout << "[OK] Reglas actualizadas correctamente (" << maxHoras
                      << "h semanales / " << minDescanso << "h descanso minimo).\n";
        } else {
            std::cout << "[ERROR] Parametros invalidos. Ingrese valores positivos dentro de rangos legales.\n";
        }
    }

    void solicitarGeneracionMalla() {
        std::string periodo;
        std::cout << "\n--- Generar Malla Automatica ---\n";
        std::cout << "Ingrese el nombre del periodo (ej. Semana 1 - Nov 2026): ";
        std::getline(std::cin, periodo);
        if (periodo.empty()) {
            periodo = "Semana Actual";
        }
        MallaTurnos malla = controlador.ejecutarGeneracionMalla(periodo);
        mostrarMalla(malla);
    }

    void mostrarMalla(const MallaTurnos& malla) {
        std::cout << "\n=== MALLA GENERADA: " << malla.getPeriodo() << " ===\n";
        std::cout << std::left << std::setw(8) << "Turno"
                  << std::setw(15) << "Empleado"
                  << std::setw(14) << "Inicio"
                  << std::setw(14) << "Fin"
                  << "Horas (Abs)\n";
        std::cout << "-------------------------------------------------------------\n";

        int descubiertos = 0;
        for (const auto& t : malla.obtenerTurnos()) {
            if (t.getIdEmpleado() == "SIN_COBERTURA") {
                descubiertos++;
            }
            std::cout << std::left << std::setw(8) << t.getIdTurno()
                      << std::setw(15) << t.getIdEmpleado()
                      << std::setw(14) << formatearHoraSemana(t.getHoraInicio())
                      << std::setw(14) << formatearHoraSemana(t.getHoraFin())
                      << t.getDuracion() << "h (" << t.getHoraInicio() << "-" << t.getHoraFin() << ")\n";
        }
        if (descubiertos > 0) {
            std::cout << "[ADVERTENCIA] Existen " << descubiertos
                      << " franja(s) sin cobertura por restriccion legal o falta de personal.\n";
        }
    }

    void probarValidacionManual() {
        std::string idEmp;
        int inicio = 0;
        int fin = 0;

        std::cout << "\n--- Validacion Manual de Turno ---\n";
        std::cout << "ID del empleado (ej. EMP-01): ";
        std::getline(std::cin, idEmp);
        std::cout << "Hora de inicio en la semana (0 a 167, ej. 16 = Lun 16:00): ";
        if (!(std::cin >> inicio)) {
            limpiarEntrada();
            std::cout << "[ERROR] Hora de inicio invalida.\n";
            return;
        }
        std::cout << "Hora de fin en la semana (1 a 168, ej. 24 = Mar 00:00): ";
        if (!(std::cin >> fin)) {
            limpiarEntrada();
            std::cout << "[ERROR] Hora de fin invalida.\n";
            return;
        }
        limpiarEntrada();

        std::string resultado = controlador.validarAsignacionManual(idEmp, inicio, fin);
        std::cout << ">> " << resultado << "\n";
    }

    void mostrarEmpleados() {
        std::cout << "\n--- Plantilla de Empleados ---\n";
        for (const auto& emp : controlador.listarEmpleados()) {
            std::cout << " - " << emp.obtenerInfo() << "\n";
        }
    }
};

#endif