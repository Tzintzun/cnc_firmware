#ifndef __INTERPRETE__
#define __INTERPRETE__
#include "gcode.h"
#include "interprete.h"
#include "errores.h"
#include <iostream>
#include <sstream> 
#include <cmath>
#include <string>
#include <cstring>
#include <queue>
#include <list>

#define FAIL_INTERPRETE(x) *error=x; return std::queue<Instruccion*>()
class InterpreteExcepcion {
    public:

    private:
    std::string mensaje;
};
//bool leer_flotante(std::string linea, int* indice, double* valor);

typedef struct{ //Los ejes de movimiento deben de estar al principio de la estrucutra
    double x; //eje x
    double y; //eje y
    double z; //eje z
    int N; // numero de linea
    double f; // feedrate
    int bandera_palabras;
}gcode_valores;

class Instruccion{
    //gcode_valores valores;
    unsigned int instruccion;

    public:
        gcode_valores valores;
        void setInstruccion(unsigned int instruccion);
        unsigned int getInstruccion();
        Instruccion();
        std::string toString();
};

class Interprete
{
    public:
    std::queue<Instruccion*> interpretar_bloque_gcode(std::string linea, int* error);
    
};





#endif