#include <iostream>
#include <string>
#include <queue>
#include <readline/readline.h>
#include <readline/history.h>

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
            gcode_bloque = readline(">>> ");
            std::cout<<gcode_bloque<<std::endl;
            if(gcode_bloque.empty()){
                continue;
            }else{
                add_history(gcode_bloque.c_str());
            }
            respuesta = maquina.ejecutar_instruccion(gcode_bloque);
            if( respuesta != OK ){
                std::cout<<"\033[31m"<<obtener_error(respuesta, gcode_bloque)<<"\033[0m"<<std::endl;
            }

        }
        
    }else{
        std::string nombre_archivo(argv[1]);
        std::cout<<nombre_archivo<<std::endl;

        int resultado  = maquina.ejecutar_archivo(nombre_archivo);
        if(resultado != OK){
            if(resultado == PROGRAMA_TERMINADO){
                std::cout<<"PROGRMA FINALIZADO"<<std::endl;
            }else{
                std::cout<<"PROGRMA FINALIZADO CON ERRORES"<<std::endl;
            }
            

        }
    }
   
    return 0;
}
