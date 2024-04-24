#ifndef __INTERPRETE__
#define __INTERPRETE__
#include "gcode.h"
#include <string>
#include <exception>


class InterpreteExcepcion : public std::exception{
    public:
        InterpreteExcepcion(std::string mensaje){
            this->mensaje = mensaje;
        }
         const char* what() const noexcept override {
            return mensaje.c_str();
        }
    private:
    std::string mensaje;
};
//bool leer_flotante(std::string linea, int* indice, double* valor);

typedef struct{
    double x; //eje x
    double y; //eje y
    double z; //eje z
    int N; // numero de linea
    double f; // feedrate
}gcode_valores;

class Instruccion{
    //gcode_valores valores;
    unsigned int instruccion;

    public:
        gcode_valores valores;
        void setInstruccion(unsigned int instruccion);
};

class Interprete
{
    public:
    Instruccion* interpretar_bloque_gcode(std::string linea, int* error);
    
};





#endif