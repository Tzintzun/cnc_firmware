#ifndef __ACTUADORES__
#define __ACTUADORES__

#include <cmath>
#include <ctime>
#include <csignal>
#include <map>
#include <iostream>
#include <chrono>
#include <thread>
#include <wiringPi.h>

#include "inih/cpp/INIReader.h"
#include "trayectorias.h"
#include "configuracion.h"
#include "pines.h"
#include "errores.h"
#include "herramienta.h"

#define FAIL_MANIPULACION_ACTUADOR(x) return x

typedef struct 
{
    long numero_pasos;
    long periodo;
    int pin;
    timer_t *timer;
    bool estado;
}configuracion_actuador;



class ManipularActuadores{
    private:
    static std::map<int, configuracion_actuador *> actuadores;
    static bool temporizadores_listos;
    int pin_eje[NUM_EJES];
    int pin_dir_ejes[NUM_EJES];
    int pin_habilitar_ejes;
    int pin_habilitar_herramienta;
    int pin_dir_herramienta;
    Herramienta *router;
    static void signal_handler(int signum, siginfo_t *info, void *context);

    public:
    int ejecutar_movimiento(parametros_actuadores parametros);
    void establecer_herramienta_sentido_horario();
    void establecer_herramienta_sentido_antihorario();
    void habilitar_herramienta();
    void deshabilitar_herramienta();
    ManipularActuadores(INIReader reader_config);

};



#endif
