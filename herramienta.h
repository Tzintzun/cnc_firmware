#ifndef __HERRAMIENTA__
#define __HERRAMIENTA__

#include "errores.h"
#include "pines.h"
class Herramienta{

    private:
    double feedrate; 
    bool router_dir; // M3(true, sentido horario)/M4(faslse, sentido antihorario) direccion del router .
    bool estado; //M5 herramienta detenida(false)- herramienta encendida (true).

    public:
    Herramienta();
    int cambiar_direccion(int pin, bool direccion);
    int cambiar_estado(int pin, bool estado);

};

#endif