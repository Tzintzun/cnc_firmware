#ifndef __HERRAMIENTA__
#define __HERRAMIENTA__

#include <errores.h>
#include <pines.h>
class Herramienta{

    private:
    double feedrate; 
    bool router_dir; // M3/M4 direccion del router.
    bool estado; //M5 herramienta detenida.

    public:
    Herramienta();
    int cambiar_direccion(int pin, bool direccion);
    int cambiar_estado(int pin, bool estado);

};

#endif