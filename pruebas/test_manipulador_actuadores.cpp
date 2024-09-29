/*Compilacion: g++ inih/ini.c inih/cpp/INIReader.cpp trayectorias.cpp actuadores.cpp interprete.cpp errores.cpp pruebas/test_manipulador_actuadores.cpp -o pruebas/test_manipulador_actuadores.exe -l wiringPi*/

#include <iostream>
#include <cmath>


#include "../inih/cpp/INIReader.h"
#include "../configuracion.h"
#include "../errores.h"
#include "../trayectorias.h"
#include "../actuadores.h"


int main(void){

    INIReader reader("./cnc.ini");
    if(reader.ParseError() < 0){
        std::cerr<<"ERROR ["<<reader.ParseError()<<"]: "<<"No se pudo cargar el archivo de configuracion"<<std::endl;
        return -1;
    }

    ManipularActuadores *manipulador = new ManipularActuadores(reader);

    parametros_actuadores parametros_prueba;
    for(int i=0; i<NUM_EJES; i++){
        parametros_prueba.direccion[i] = true;
        parametros_prueba.num_pasos[i] = 200*(i+1);
        parametros_prueba.periodo_pasos[i] = (long)floorl(250000000L/(i+1));
    }

    
    int error = manipulador->ejecutar_movimiento(parametros_prueba);
    if( error != OK){
        std::cout<<obtener_error(error, "TEST ManipularActuadores")<<std::endl;
    }

    std::cout<<"FIN TEST ManipularActuadores"<<std::endl;
    
    
    return 0;
}