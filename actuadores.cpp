#include "actuadores.h"
#include "pines.h"


ManipularActuadores::ManipularActuadores(INIReader reader_config){
    this->pin_eje[0] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_X", PIN_EJE_X);
    this->pin_eje[1] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_Y", PIN_EJE_Y);
    this->pin_eje[2] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_Z", PIN_EJE_Z);

    this->pin_dir_ejes[0] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_X", PIN_DIR_EJE_X);
    this->pin_dir_ejes[1] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_Y", PIN_DIR_EJE_Y);
    this->pin_dir_ejes[2] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_Z", PIN_DIR_EJE_Z);

    this->pin_habilitar_ejes = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_HABILITAR_EJES", PIN_HABILITAR_EJES);
}


int ManipularActuadores::ejecutar_movimiento(parametros_actuadores parametros){

    for(int i=0; i<NUM_EJES;i++){
        struct sigaction senial_timer;
        senial_timer.sa_flags = SA_SIGINFO

    }
}