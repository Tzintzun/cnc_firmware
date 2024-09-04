#include "trayectorias.h"

CalculadoraTrayectorias::CalculadoraTrayectorias(INIReader reader_conf){
    this->area_trabajo[0] = reader_conf.GetReal("MaquinaCNC", "MAX_X_DIM", MAX_X_DIS);
    this->area_trabajo[1] = reader_conf.GetReal("MaquinaCNC", "MAX_Y_DIM", MAX_Y_DIS);
    this->area_trabajo[2] = reader_conf.GetReal("MaquinaCNC", "MAX_Z_DIM", MAX_Z_DIS);

    this->pasos_mm[0] = reader_conf.GetReal("Trayectorias", "PASOS_MM_X", PASOS_X_MM);
    this->pasos_mm[1] = reader_conf.GetReal("Trayectorias", "PASOS_MM_Y", PASOS_Y_MM);
    this->pasos_mm[2] = reader_conf.GetReal("Trayectorias", "PASOS_MM_Z", PASOS_Z_MM);
}

std::string CalculadoraTrayectorias::toString(){
    std::string calculadora = "\nPASOS x MM: ";

    for(int i  = 0; i<NUM_EJES; i++){
        calculadora += "\t" + std::to_string(this->pasos_mm[i]);
    }
    return calculadora;
}