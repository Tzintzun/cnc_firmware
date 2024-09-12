
#include "maquina.h"
#include "inih/cpp/INIReader.h"
#include "configuracion.h"

MaquinaCNC::MaquinaCNC(INIReader reader){


    /*CargarConfiguracion*/
    (this->calculadora) = new CalculadoraTrayectorias(reader);
    this->dimenciones_xyz[0] = reader.GetReal("MaquinaCNC", "MAX_X_DIM", MAX_X_DIS);
    this->dimenciones_xyz[1] = reader.GetReal("MaquinaCNC", "MAX_Y_DIM", MAX_Y_DIS);
    this->dimenciones_xyz[2] = reader.GetReal("MaquinaCNC", "MAX_Z_DIM", MAX_Z_DIS);
    memset(this->posicion_xyz,0, sizeof(posicion_xyz));

}

std::string MaquinaCNC::toString(){
    std::string maquina = "\nNUMERO EJES: " + std::to_string(NUM_EJES);
    maquina += "\nDIMENCIONES: ";
    for(int i = 0; i<NUM_EJES; i++){
        maquina += std::to_string(this->dimenciones_xyz[i]) + ",";
    }
    maquina += calculadora->toString();
    
    return maquina;
}