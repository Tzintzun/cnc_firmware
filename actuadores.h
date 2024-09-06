#ifndef __ACTUADORES__
#define __ACTUADORES__

#include <time.h>
#include <signal.h>
#include <map>

#include "trayectorias.h"
#include "configuracion.h"
#include "inih/cpp/INIReader.h"

class ManipularActuadores{
    private:
    std::map<int, configuracion_actuador> actuadores;
    int pinout[NUM_EJES]; // {X,Y,Z}
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
    int ejecutar_movimiento();
    ManipularActuadores(INIReader reader_config);

};

typedef struct 
{
    parametros_actuador parametros;
    int pin;
    bool estado;
}configuracion_actuador;


#endif