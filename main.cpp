#include <iostream>
#include <string>

#include "interprete/interprete.h"
#include "errores.h"

int main(int argc, char const *argv[])
{
    std::string gcode_bloque;
    Interprete* interprete_gcode = new Interprete();
    
    if(argc<2){
        std::cout<<"G-code interprete"<<std::endl;
        int error = OK;
        while (true)
        {
            std::cout<<">>>";
            std::getline(std::cin,gcode_bloque);
            std::cout<<gcode_bloque<<std::endl;
            if(! interprete_gcode->interpretar_bloque_gcode(gcode_bloque, &error) ){
                std::cout<<obtener_error(error, gcode_bloque)<<std::endl;
            };
        }
        
    }
   
    return 0;
}
