#ifndef __MAQUINA__
#define __MAQUINA__

#include "herramienta.h"
unsigned int ERROR;
typedef struct{

} instruccion_t;

class maquina_cnc{
    //posicion actual de la maquina.
    double posicion_x;
    double posicion_y;
    double posicion_z; 
    bool sistema_unidades; // G20/G21 mm / pulgadas
    bool modo_desplazamiento; //G90/G91 deplazamiento absoluto o relativo
    unsigned long numero_lineas;
    Herramienta router;
};


#endif