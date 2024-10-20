#ifndef __MAQUINA__
#define __MAQUINA__

#include <iostream>
#include <queue>
#include <fstream>
#include <chrono>
#include <thread>
#include <future>

#include "herramienta.h"
#include "interprete.h"
#include "trayectorias.h"
#include "configuracion.h"
#include "inih/cpp/INIReader.h"
#include "errores.h"
#include "actuadores.h"

#define BUFFER_INSTRUCCIONES_MAX 10
#define BUFFER_INSTRUCCIONES_MIN 5
typedef struct{

} instruccion_t;

class MaquinaCNC{
    
    private:
    double dimenciones_xyz[NUM_EJES];
    bool sistema_unidades; // G20/G21 mm/pulgadas false/true
    bool modo_desplazamiento; //G90/G91 deplazamiento absoluto(True) o incremental(False)
    unsigned long numero_lineas;

    Herramienta *router;
    Interprete *interprete_gcode;
    CalculadoraTrayectorias *calculadora;
    ManipularActuadores *manipulador_actuadores;

    
   
    std::queue<Instruccion *> * cola_ejecucion;
    
    
    
    int ejecutar_instruccion(Instruccion *instruccion);

    public:
    double posicion_xyz[NUM_EJES];
    int ejecutar_instruccion(std::string instruccion);
    int ejecutar_archivo(std::string ruta);
    MaquinaCNC(INIReader reader);
    std::string toString();
};


#endif