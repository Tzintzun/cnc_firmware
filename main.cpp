#include <iostream>
#include <string>
#include <queue>

#include "interprete.h"
#include "errores.h"
#include "inih/cpp/INIReader.h"
#include "maquina.h"

int main(int argc, char const *argv[])
{
    std::string gcode_bloque;
    INIReader reader("./cnc.ini");
    if(reader.ParseError() < 0){
        std::cout<<"ERROR ["<<reader.ParseError()<<"]: "<<"No se pudo cargar el archivo de configuracion"<<std::endl;
        return -1;
    }
    MaquinaCNC maquina(reader);

    std::cout<<maquina.toString()<<std::endl;
    /*Interprete* interprete_gcode = new Interprete();
    std::queue<Instruccion *> cola_instrucciones;
    std::queue<Instruccion *> cola_instrucciones_aux;
    if(argc<2){
        std::cout<<"MAQUINA CNC:"<<std::endl;
        int error = OK;
        while (true)
        {
            std::cout<<">>>";
            std::getline(std::cin,gcode_bloque);
            std::cout<<gcode_bloque<<std::endl;
            cola_instrucciones_aux = interprete_gcode->interpretar_bloque_gcode(gcode_bloque, &error);
            if( cola_instrucciones_aux.empty() ){
                std::cout<<obtener_error(error, gcode_bloque)<<std::endl;
            }

            while(!cola_instrucciones_aux.empty()){
                Instruccion* instruccion = cola_instrucciones_aux.back();
                std::cout<<"Instruccion: "<<instruccion->getInstruccion()<<"\t Banderas: ";
                std::cout<<std::hex<<instruccion->valores.bandera_palabras;
                std::cout<<"\n\tX: "<<instruccion->valores.x;
                std::cout<<"\n\tY: "<<instruccion->valores.y;
                std::cout<<"\n\tZ: "<<instruccion->valores.z;
                std::cout<<"\n\tF: "<<instruccion->valores.f;
                std::cout<<"\n\tN: "<<instruccion->valores.N<<"\n\n"<<std::endl;

                cola_instrucciones_aux.pop();
            }
        }
        
    }*/
   
    return 0;
}
