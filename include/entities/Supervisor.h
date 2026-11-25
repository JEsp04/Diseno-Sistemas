#ifndef SUPERVISOR_H
#define SUPERVISOR_H

#include "Usuario.h"
#include <string>

class Supervisor : public Usuario {
private:
    std::string area;

public:
    Supervisor(std::string id, std::string nombre, std::string area)
        : Usuario(std::move(id), std::move(nombre), "Supervisor"), area(std::move(area)) {}

    std::string obtenerInfo() const override {
        return "[" + id + "] " + nombre + " | Rol: " + rolSistema + " | Area: " + area;
    }
};

#endif