#include <iostream>
#include <string>
#include <queue>

#include "../interprete.h"
#include "../errores.h"
#include "../gcode.h"

struct EjemploErrores {
    std::string boque;
    int codigo_error;
};

int main(void){

    /*Comandos de prueba*/
    EjemploErrores ejemplos_errores[] = {
        {"G0 X1 Y2 Z3 Z4", ERROR_ARGUMENTO_REPETIDO},                      // Z repetido
        {"G1 X5 Y2.0 G28", ERROR_MULTIPLES_INSTRUC_CON_ARGUMENTOS},        // G1 y G28 comparten argumentos
        {"G0 G1 X2 Y2", ERROR_GRUPO_MODAL_CONFLICTO},                      // G0 y G1 del mismo grupo modal
        {"G3 X1 Y1", INSTRUCCION_NO_SOPORTADA},                            // G3 no soportada
        {"H0 X1 Y2", INSTRUCCION_NO_SOPORTADA},                            // H no es una instrucción válida
        {"G91 X1.0 Y", ERROR_FORMATO_NUMERO_INCORRECTO},                   // Argumento Y sin número
        {"X2 Y1.5", ERROR_BLOQUE_SIN_INSTRUCCION},                         // Falta instrucción G o M
        {"G90 N20 N100 X1", ERROR_ARGUMENTO_REPETIDO},                       // N repetido
        {"G1 X1 Y1.5 ZZZ F100", ERROR_FORMATO_NUMERO_INCORRECTO},          // Argumento Z con formato incorrecto
        {"N100 G1 X1.5 N100", ERROR_ARGUMENTO_REPETIDO},                   // N repetido en el bloque
        {"G1 X5 Y2.2 F", ERROR_FORMATO_NUMERO_INCORRECTO},                 // Falta número en feedrate F
    };

    std::string ejemplo_correctos[] = {
        "G0 X1 Y2 Z3",            // 1.Desplazamiento lineal libre con X, Y, Z
        "G1 X0.5 Y-1 Z0 F500",     // 2.Interpolación lineal con feedrate
        "G28 X0 Y0 Z0",            // 3.De splazamiento a casa
        "N10 G20 X5.5 Y2 Z-3",     // 4.Configuración en milímetros con número de línea
        "N15 G21",                 // 5.Configuración en pulgadas
        "G90",                     // 6.Modo distancia absoluta
        "G91",                     // 7.Modo distancia incremental
        "N50 M5",                  // 8.Detener herramienta con número de línea
        "M4",                       // 9.Encender herramienta en sentido antihorario
        "N100 M3",                 // 10.Encender herramienta en sentido horario
        "N200 G1 X1 Y1 F100"      // 11.Interpolación lineal con feedrate
    };

    Interprete *interptete = new Interprete();
    std::queue<Instruccion *> cola_instrucciones;
    /*Probando errores*/
    std::cout<<"TEST de ERRORES:"<<std::endl;
    int error = OK;
    for(int i = 0; i<(sizeof(ejemplos_errores)/sizeof(EjemploErrores)) ; i++){
        cola_instrucciones = interptete->interpretar_bloque_gcode(ejemplos_errores[i].boque, &error);
        if(cola_instrucciones.size()== 0){
            if(error == ejemplos_errores[i].codigo_error){
                std::cout<<"Test "<<i<<": "<<ejemplos_errores[i].boque<<" ---> OK"<<std::endl;
                error = OK;
                continue;
            }
        }
        
        std::cout<<"Test "<<i<<": "<<ejemplos_errores[i].boque<<" ---> ERROR: "<<error<<std::endl;
        while(!cola_instrucciones.empty()){
            Instruccion * aux = cola_instrucciones.back();
            std::cout<<ejemplos_errores[i].boque<<"{"<<std::endl;
            std::cout<<aux->toString()<<std::endl;
            std::cout<<"}"<<std::endl;
            cola_instrucciones.pop();
        }
        error = OK;
}


    std::cout<<std::endl<<std::endl<<"TEST de funcionamiento"<<std::endl;

    error = OK;
    for(int i = 0; i<11 ; i++){
        cola_instrucciones = interptete->interpretar_bloque_gcode(ejemplo_correctos[i], &error);
        if(cola_instrucciones.size() != 0){
            if(error == OK){
                while(!cola_instrucciones.empty()){
                    Instruccion * aux = cola_instrucciones.back();
                    std::cout<<"Test "<<i<<": "<<ejemplo_correctos[i]<<"{"<<std::endl;
                    std::cout<<aux->toString()<<std::endl;
                    std::cout<<"}"<<std::endl;
                    cola_instrucciones.pop();
                }
                continue;
            }
        }
        std::cout<<"Test "<<i<<": "<<ejemplo_correctos[i]<<" ---> ERROR "<<error<<std::endl;
        error = OK;
    }

    return 0;
}