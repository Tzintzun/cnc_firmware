#ifndef __ACTUADORES__
#define __ACTUADORES__

#include <ctime>
#include <csignal>
#include <map>
#include <iostream>
#include <chrono>
#include <thread>
//#include <wiringPi.h>

#include "inih/cpp/INIReader.h"
#include "trayectorias.h"
#include "configuracion.h"
#include "pines.h"
#include "errores.h"

#define FAIL_MANIPULACION_ACTUADOR(x) return x

typedef struct 
{
    long numero_pasos;
    long periodo;
    int pin;
    timer_t *timer;
}configuracion_actuador;



class ManipularActuadores{
    private:
    static std::map<int, configuracion_actuador *> actuadores;
    static bool temporizadores_listos;
    int pin_eje[NUM_EJES];
    int pin_dir_ejes[NUM_EJES];
    int pin_habilitar_ejes;
    
    static void signal_handler(int signum, siginfo_t *info, void *context);

    public:
    int ejecutar_movimiento(parametros_actuadores parametros);
    ManipularActuadores(INIReader reader_config);

};



#endif
