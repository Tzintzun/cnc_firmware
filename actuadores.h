#ifndef __ACTUADORES__
#define __ACTUADORES__

#include <ctime>
#include <csignal>


#include <map>

#include "trayectorias.h"
#include "configuracion.h"
#include "inih/cpp/INIReader.h"

typedef struct 
{
    
}configuracion_actuadores;



class ManipularActuadores{
    private:
    std::map<int, configuracion_actuadores> actuadores;
    int pin_eje[NUM_EJES];
    int pin_dir_ejes[NUM_EJES];
    int pin_habilitar_ejes;

    void signal_handler(int signum, siginfo_t *info, void context){
        if(info->si_value.sival_ptr){
            int id_timer = *(reinterpret_cast<int*>(info->si_value.sival_ptr));
            configuracion_actuador aux = actuadores[id_timer];
            /*
            if(aux.estado != true){
                digitalWrite(aux.pin, HIGH);
                digitalWrite(aux.pin, LOW);
                aux.parametros->num_pasos -= 1;
                if(aux.parametros->num_pasos <= 0){
                    aux.estado = true;
                }
            }
            */
        }
        
    }

    public:
    int ejecutar_movimiento(parametros_actuadores parametros);
    ManipularActuadores(INIReader reader_config);

};



#endif