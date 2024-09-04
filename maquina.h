#ifndef __MAQUINA__
#define __MAQUINA__


#include <queue>

#include "herramienta.h"
#include "interprete.h"
#include "trayectorias.h"
#include "configuracion.h"
#include "inih/cpp/INIReader.h"

typedef struct{

} instruccion_t;

class MaquinaCNC{
    //posicion actual de la maquina.
    double posicion_xyz[NUM_EJES];
    double dimenciones_xyz[NUM_EJES];
    bool sistema_unidades; // G20/G21 mm / pulgadas
    bool modo_desplazamiento; //G90/G91 deplazamiento absoluto o relativo
    unsigned long numero_lineas;

    Herramienta *router;
    Interprete *interprete_gcode;
    CalculadoraTrayectorias *calculadora;
    //ManipuladorActuadores actuadores;
    std::queue<Instruccion> cola_instrucciones;

    public:
    int ejecutar_instruccion(std::string instruccion);
    int ejecutar_archivo(std::string ruta);
    MaquinaCNC(INIReader reader);
    std::string toString();
};


#endif