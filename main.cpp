#include <iostream>
#include <string>
#include <queue>


#include "interprete.h"
#include "errores.h"
#include "inih/cpp/INIReader.h"
#include "maquina.h"
#include "trayectorias.h"

int main(int argc, char const *argv[]) //
{
    std::string gcode_bloque;
    INIReader reader("./cnc.ini");
    if(reader.ParseError() < 0){
        std::cerr<<"ERROR ["<<reader.ParseError()<<"]: "<<"No se pudo cargar el archivo de configuracion"<<std::endl;
        return -1;
    }
    MaquinaCNC maquina(reader);

    std::cout<<maquina.toString()<<std::endl;
    
    if(argc<2){
        std::cout<<"MAQUINA CNC:"<<std::endl;
        int respuesta = OK;
        while (true)
        {
            std::cout<<">>>";
            std::getline(std::cin,gcode_bloque);
            std::cout<<gcode_bloque<<std::endl;
            if(gcode_bloque.empty()){
                continue;
            }
            respuesta = maquina.ejecutar_instruccion(gcode_bloque);
            if( respuesta != OK ){
                std::cout<<obtener_error(respuesta, gcode_bloque)<<std::endl;
            }

        }
        
    }else{
        std::string nombre_archivo(argv[1]);
        std::cout<<nombre_archivo<<std::endl;
    }
   
    return 0;
}
