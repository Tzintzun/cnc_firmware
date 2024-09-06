#include "actuadores.h"


ManipularActuadores::ManipularActuadores(INIReader reader_config){
    this->pinout[0] = reader_config.GetInteger("PINOUT", "PIN_EJE_X", PIN_EJE_X);
    this->pinout[0] = reader_config.GetInteger("PINOUT", "PIN_EJE_Y", PIN_EJE_Y);
    this->pinout[0] = reader_config.GetInteger("PINOUT", "PIN_EJE_Z", PIN_EJE_Z);
}