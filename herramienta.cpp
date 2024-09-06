#include "herramienta.h"
#include "errores.h"

Herramienta::Herramienta(){
    this->estado = false;
}

int Herramienta::cambiar_direccion(bool direccion){
    this->router_dir = direccion;

    if(this->router_dir == false){
        //habilitamos pin de direccion;
    }

    if(this->router_dir == true){
        //deshabilitamos pin de cambio de direccion
    }
    return OK;
}

int Herramienta::cambiar_estado(bool estado){
    this->estado = estado;

    if(this->estado == false){
        //deshabilitamos herramienta
    }
    if(this->estado == true){
        //habilitamos herramienta
    }

    return OK;
}