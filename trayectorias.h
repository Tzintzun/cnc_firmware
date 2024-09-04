#ifndef __ACTUADORES__
#define __ACTUADORES__

#include "configuracion.h"
#include "inih/cpp/INIReader.h"

typedef struct 
{
    long num_pasos;
    double periodo_pasos;
}parametros_actuador;

class CalculadoraTrayectorias{
    private:
    long pasos_mm[NUM_EJES]; //pasos por milimetro
    double area_trabajo [NUM_EJES];

    public:
    CalculadoraTrayectorias(INIReader reader_conf);
    std::string toString();
    
};

#endif