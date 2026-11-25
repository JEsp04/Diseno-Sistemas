#ifndef EMPLEADO_H
#define EMPLEADO_H

#include "Usuario.h"
#include <string>

class Empleado : public Usuario {
private:
    std::string cargoOperativo;
    int horasAcumuladas;

public:
    Empleado(std::string id, std::string nombre, std::string cargo)
        : Usuario(std::move(id), std::move(nombre), "Empleado"),
          cargoOperativo(std::move(cargo)),
          horasAcumuladas(0) {}

    void sumarHoras(int horas) {
        if (horas > 0) {
            horasAcumuladas += horas;
        }
    }

    void reiniciarHoras() { horasAcumuladas = 0; }
    int obtenerHorasAcumuladas() const { return horasAcumuladas; }
    std::string getCargo() const { return cargoOperativo; }

    std::string obtenerInfo() const override {
        return "[" + id + "] " + nombre + " | Cargo: " + cargoOperativo +
               " | Horas acumuladas: " + std::to_string(horasAcumuladas) + "h";
    }
};

#endif