#ifndef __TRAYECTORIAS__
#define __TRAYECTORIAS__

#include <cstring>
#include <cmath>

#include "gcode.h"
#include "interprete.h"
#include "configuracion.h"
#include "inih/cpp/INIReader.h"


#define FAIL_CALCULO_TRAYECTORIA(x) *error=x;  memset(&parametros,0,sizeof(parametros_actuadores)); return parametros

typedef struct 
{
    long num_pasos[NUM_EJES];
    long periodo_pasos[NUM_EJES];
    bool direccion[NUM_EJES]; //false: direccio antihorario; true: direcion horario;
}parametros_actuadores;

class CalculadoraTrayectorias{
    private:
    long pasos_mm[NUM_EJES]; //pasos por milimetro
    double area_trabajo [NUM_EJES];
    double feedrate_desplazamiento;
    public:
    parametros_actuadores calcular_trayectoria_lineal(Instruccion instruccion, double *posicion, bool unidades, bool sistema_cordenadas, int* error);
    CalculadoraTrayectorias(INIReader reader_conf);
    std::string toString();
    
};

#endif