#include <iostream>
#include <string>
#include <queue>

#include "interprete/interprete.h"
#include "errores.h"

int main(int argc, char const *argv[])
{
    std::string gcode_bloque;
    Interprete* interprete_gcode = new Interprete();
    std::queue<Instruccion *> cola_instrucciones;
    std::queue<Instruccion *> cola_instrucciones_aux;
    if(argc<2){
        std::cout<<"G-code interprete"<<std::endl;
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
        
    }
   
    return 0;
}
