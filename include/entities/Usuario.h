#ifndef USUARIO_H
#define USUARIO_H

#include <string>

class Usuario {
protected:
    std::string id;
    std::string nombre;
    std::string rolSistema;

public:
    Usuario(std::string id, std::string nombre, std::string rol)
        : id(std::move(id)), nombre(std::move(nombre)), rolSistema(std::move(rol)) {}

    virtual ~Usuario() = default;

    std::string getId() const { return id; }
    std::string getNombre() const { return nombre; }
    virtual std::string obtenerInfo() const = 0;
};

#endif