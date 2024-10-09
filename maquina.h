#ifndef __MAQUINA__
#define __MAQUINA__

#include <iostream>
#include <queue>

#include "herramienta.h"
#include "interprete.h"
#include "trayectorias.h"
#include "configuracion.h"
#include "inih/cpp/INIReader.h"
#include "errores.h"
#include "actuadores.h"
typedef struct{

} instruccion_t;

class MaquinaCNC{
    //posicion actual de la maquina.
   
    double dimenciones_xyz[NUM_EJES];
    bool sistema_unidades; // G20/G21 mm/pulgadas false/true
    bool modo_desplazamiento; //G90/G91 deplazamiento absoluto(True) o incremental(False)
    unsigned long numero_lineas;

    Herramienta *router;
    Interprete *interprete_gcode;
    CalculadoraTrayectorias *calculadora;
    ManipularActuadores *manipulador_actuadores;
    //ManipuladorActuadores actuadores;
    std::queue<Instruccion *> cola_instrucciones;

    public:
     double posicion_xyz[NUM_EJES];
    int ejecutar_instruccion(std::string instruccion);
    int ejecutar_archivo(std::string ruta);
    MaquinaCNC(INIReader reader);
    std::string toString();
};


#endif