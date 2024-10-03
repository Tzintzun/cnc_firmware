#include "herramienta.h"
#include "errores.h"

Herramienta::Herramienta(){
    this->estado = false;
}

    int Herramienta::cambiar_direccion(int pin, bool direccion){
        this->router_dir = direccion;

        if(this->router_dir == false){
            ESTABLECER_SENTIDO_HORARIO_HERRAMIENTA(pin);
        }

        if(this->router_dir == true){
            ESTABLECER_SENTIDO_ANTIHORARIO_HERRAMIENTA(pin);
        }
        return OK;
    }

    int Herramienta::cambiar_estado(int pin, bool estado){
        this->estado = estado;

        if(this->estado == false){
            DESHABILITAR_HERRAMIENTA(pin);
        }
        if(this->estado == true){
            HABILITAR_HERRAMIENTA(pin);
        }

        return OK;
    }