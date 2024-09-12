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
    Interprete* interprete_gcode = new Interprete();
    CalculadoraTrayectorias* calculadora = new CalculadoraTrayectorias(reader);
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
            error = OK;
            if(gcode_bloque.empty()){
                continue;
            }
            cola_instrucciones_aux = interprete_gcode->interpretar_bloque_gcode(gcode_bloque, &error);
            if( error != OK ){
                std::cout<<obtener_error(error, gcode_bloque)<<std::endl;
            }

            while(!cola_instrucciones_aux.empty()){
                Instruccion* instruccion = cola_instrucciones_aux.back();
                std::cout<<"Instruccion: "<<instruccion->getInstruccion()<<"\tBanderas: ";
                std::cout<<std::hex<<instruccion->valores.bandera_palabras;
                std::cout<<"\n\tX: "<<instruccion->valores.x;
                std::cout<<"\n\tY: "<<instruccion->valores.y;
                std::cout<<"\n\tZ: "<<instruccion->valores.z;
                std::cout<<"\n\tF: "<<instruccion->valores.f;
                std::cout<<"\n\tN: "<<instruccion->valores.N<<"\n\n"<<std::endl;

                if(instruccion->getInstruccion() == DESPLAZAMIENTO_LINEAL_LIBRE ||
                    instruccion->getInstruccion() == INTERPOLACION_LINEAL){
                        parametros_actuadores parametros = calculadora->calcular_trayectoria_lineal(*instruccion, maquina.posicion_xyz,false,&error );
                        if(error != OK){
                            std::cout<<obtener_error(error, gcode_bloque)<<std::endl;
                        }
                        std::cout<<"Parametros: "<<std::endl;
                        std::cout<<"\tDIRECCION: "<<std::endl;
                        for(int i=0; i<NUM_EJES;i++){
                            std::cout<<std::dec<<"\t\t"<<parametros.direccion[i]<<std::endl;
                        }
                        std::cout<<"\tNUMERO de PASOS: "<<std::endl;
                        for(int i=0; i<NUM_EJES;i++){
                            std::cout<<std::dec<<"\t\t"<<parametros.num_pasos[i]<<std::endl;
                        }
                        std::cout<<"\tPERIODO PASOS: "<<std::endl;
                        for(int i=0; i<NUM_EJES;i++){
                            std::cout<<std::dec<<"\t\t"<<parametros.periodo_pasos[i]<<std::endl;
                        }
                    }
                cola_instrucciones_aux.pop();
            }
        }
        
    }else{
        std::string nombre_archivo(argv[1]);
        std::cout<<nombre_archivo<<std::endl;
    }
   
    return 0;
}
