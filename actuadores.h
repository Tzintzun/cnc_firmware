#ifndef __ACTUADORES__
#define __ACTUADORES__

#include <ctime>
#include <csignal>
#include <map>

#include "trayectorias.h"
#include "configuracion.h"
#include "inih/cpp/INIReader.h"
#include "pines.h"
#include "errores.h"

#define FAIL_MANIPULACION_ACTUADOR(x) return x

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
        int timer_id = -1;
        if(info->si_value.sival_ptr){
            timer_id = *(reinterpret_cast<int*>(info->si_value.sival_ptr));
        }
        
    }

    public:
    int ejecutar_movimiento(parametros_actuadores parametros);
    ManipularActuadores(INIReader reader_config);

};



#endif