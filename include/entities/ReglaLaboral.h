#ifndef REGLA_LABORAL_H
#define REGLA_LABORAL_H

class ReglaLaboral {
private:
    int maxHorasSemanales;
    int minDescansoHoras;

public:
    ReglaLaboral(int maxHoras = 47, int minDescanso = 12)
        : maxHorasSemanales(maxHoras), minDescansoHoras(minDescanso) {}

    int getMaxHorasSemanales() const { return maxHorasSemanales; }
    int getMinDescansoHoras() const { return minDescansoHoras; }
};

#endif