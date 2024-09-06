#ifndef __HERRAMIENTA__
#define __HERRAMIENTA__

class Herramienta{

    private:
    double feedrate; 
    bool router_dir; // M3/M4 direccion del router.
    bool estado; //M5 herramienta detenida.

    public:
    Herramienta();
    int cambiar_direccion(bool direccion);
    int cambiar_estado(bool estado);

};

#endif