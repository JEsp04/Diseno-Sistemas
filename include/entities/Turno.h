#ifndef TURNO_H
#define TURNO_H

#include <string>

class Turno {
private:
    std::string idTurno;
    std::string idEmpleado;
    int horaInicio;
    int horaFin;
    int duracionHoras;

public:
    Turno(std::string id, std::string idEmp, int inicio, int fin)
        : idTurno(std::move(id)),
          idEmpleado(std::move(idEmp)),
          horaInicio(inicio),
          horaFin(fin),
          duracionHoras(fin - inicio) {}

    std::string getIdTurno() const { return idTurno; }
    std::string getIdEmpleado() const { return idEmpleado; }
    int getHoraInicio() const { return horaInicio; }
    int getHoraFin() const { return horaFin; }
    int getDuracion() const { return duracionHoras; }
};

#endif