/*Compilacion: inih/ini.c inih/cpp/INIReader.cpp trayectorias.cpp interprete.cpp errores.cpp pruebas/test_calculadora_trayectorias.cpp -o pruebas/testest_calculadora_trayectorias*/

#include "../trayectorias.h"
#include "../interprete.h"
#include "../configuracion.h"
#include "../errores.h"
#include "../inih/cpp/INIReader.h"

typedef struct {
    Instruccion instruccion;
    //double posicion_inicial[NUM_EJES];
    bool unidades; // false para mm, true para pulgadas
    bool sistema_movimiento;
    parametros_actuadores resultado_esperado;
    int error_esperado;
} PruebaTrayectoria;

int main(void){
    
    std::vector<PruebaTrayectoria> pruebas_exito;
    std::vector<PruebaTrayectoria> pruebas_error;

    INIReader reader("./cnc.ini");
    if(reader.ParseError() < 0){
        std::cerr<<"ERROR ["<<reader.ParseError()<<"]: "<<"No se pudo cargar el archivo de configuracion"<<std::endl;
        //return -1;
    }

    CalculadoraTrayectorias *calculadora = new CalculadoraTrayectorias(reader);

    long posicion[NUM_EJES] = {0,0,0};
    Instruccion instruccion1;
    instruccion1.setInstruccion(INTERPOLACION_LINEAL); // G1
    instruccion1.valores.x = 10000;
    instruccion1.valores.y = 5000;
    instruccion1.valores.z = 1000;
    instruccion1.valores.bandera_palabras = X_PALABRA | Y_PALABRA | Z_PALABRA;

    PruebaTrayectoria prueba1 = {
        instruccion1,
        //{0.0, 0.0, 0.0}, // Posición inicial
        false, // Unidades en mm
        true, // movimiento absoluto
        { // Resultado esperado
            {22000, 11000, 2200}, // num_pasos
            {153067, 306135, 1530678}, // periodo_pasos (esto depende del feedrate y la distancia)
            {true, true, true} // dirección
        },
        OK // No se espera error
    };

    pruebas_exito.push_back(prueba1);

    Instruccion instruccion2;
    instruccion2.setInstruccion(DESPLAZAMIENTO_LINEAL_LIBRE); // G0
    instruccion2.valores.x = 400; // pulgadas
    instruccion2.valores.y = 200;
    instruccion2.valores.bandera_palabras = X_PALABRA | Y_PALABRA;

    PruebaTrayectoria prueba2 = {
        instruccion2,
        //{50.0, 50.0, 0.0}, // Posición inicial
        true, // Unidades en pulgadas
        true, // movimiento absoluto
        { // Resultado esperado
            {257047, 268223, 0}, // num_pasos (conversión a mm)
            {7759, 7435, 0}, // periodo_pasos
            {false, false, true} // dirección
        },
        OK
    };

    pruebas_exito.push_back(prueba2);

    Instruccion instruccion3;
    instruccion3.setInstruccion(DESPLAZAMIENTO_LINEAL_LIBRE); // G0
    instruccion3.valores.x = -100; // pulgadas
    instruccion3.valores.y = 0;
    instruccion3.valores.bandera_palabras = X_PALABRA | Y_PALABRA;

    PruebaTrayectoria prueba3 = {
        instruccion3,
        //{50.0, 50.0, 0.0}, // Posición inicial
        true, // Unidades en pulgadas
        true, // movimiento absoluto
        { // Resultado esperado
            {257047, 268223, 0}, // num_pasos (conversión a mm)
            {7759, 7435, 0}, // periodo_pasos
            {false, false, true} // dirección
        },
        OK
    };

    pruebas_exito.push_back(prueba3);

    Instruccion instruccion4;
    instruccion4.setInstruccion(DESPLAZAMIENTO_LINEAL_LIBRE); // G0
    instruccion4.valores.x = 10000; 
    instruccion4.valores.y = 2730;
    instruccion4.valores.bandera_palabras = X_PALABRA | Y_PALABRA;

    PruebaTrayectoria prueba4 = {
        instruccion4,
        //{50.0, 50.0, 0.0}, // Posición inicial
        false, // Unidades en mm
        true, // movimiento absoluto
        { // Resultado esperado
            {257047, 268223, 0}, // num_pasos (conversión a mm)
            {7759, 7435, 0}, // periodo_pasos
            {false, false, true} // dirección
        },
        OK
    };

    pruebas_exito.push_back(prueba4);

    Instruccion instruccion5;
    instruccion5.setInstruccion(DESPLAZAMIENTO_LINEAL_LIBRE); // G0
    instruccion5.valores.x = 10000; 
    instruccion5.valores.y = 2730;
    instruccion5.valores.bandera_palabras = X_PALABRA | Y_PALABRA;

    PruebaTrayectoria prueba5 = {
        instruccion5,
        //{50.0, 50.0, 0.0}, // Posición inicial
        false, // Unidades en mm
        false, // movimiento relativo
        { // Resultado esperado
            {257047, 268223, 0}, // num_pasos (conversión a mm)
            {7759, 7435, 0}, // periodo_pasos
            {false, false, true} // dirección
        },
        OK
    };

    pruebas_exito.push_back(prueba5);



    Instruccion instruccion6;
    instruccion6.setInstruccion(CONF_UNIDADES_PULGADAS); // G21

    PruebaTrayectoria prueba6 = {
        instruccion6,
        //{0.0, 0.0, 0.0}, // Posición inicial
        false, // Unidades en mm (no importa en este caso)
        true, // movimiento absoluto
        {}, // No importa el valor de retorno
        ERROR_INSTRUCCION_SIN_DESPLAZAMIENTO
    };

    pruebas_error.push_back(prueba6);

    Instruccion instruccion7;
    instruccion7.setInstruccion(INTERPOLACION_LINEAL); // G1
    instruccion7.valores.x = 400.0; // Fuera del límite de área (300 mm)
    instruccion7.valores.y = 50.0;
    instruccion7.valores.bandera_palabras = X_PALABRA | Y_PALABRA;

    PruebaTrayectoria prueba7 = {
        instruccion7,
        //{0.0, 0.0, 0.0}, // Posición inicial
        false, // Unidades en mm
        true, // movimiento absoluto
        {}, // No importa el valor de retorno
        ERROR_TRAYECTORIA_FUERA_AREA
    };

    pruebas_error.push_back(prueba7);

    int num_prueba = 1;
    for ( PruebaTrayectoria& prueba : pruebas_exito) {
        int error = OK;
        calculadora = new CalculadoraTrayectorias(reader);
        parametros_actuadores resultado = calculadora->calcular_trayectoria_lineal(prueba.instruccion, posicion, prueba.unidades, prueba.sistema_movimiento, &error);
        
        std::cout<<std::endl<<"TEST"<<num_prueba<<std::endl;
        num_prueba++;
        if(error == OK){
            std::cout<<prueba.instruccion.toString()<<std::endl;
            
            std::cout<<"PASOS_MM: ";
            for(int i=0;i<NUM_EJES;i++){
                if(resultado.num_pasos[i] != prueba.resultado_esperado.num_pasos[i]){
                    std::cout<<"ERROR "<<prueba.resultado_esperado.num_pasos[i]<<" "<<resultado.num_pasos[i]<<",";
                    
                }else{
                    std::cout<<"OK,";
                }
            }
            std::cout<<std::endl;

            std::cout<<"PERIODO_PASOS: ";
            for(int i=0;i<NUM_EJES;i++){
                if(resultado.periodo_pasos[i] != prueba.resultado_esperado.periodo_pasos[i]){
                     std::cout<<"ERROR "<<prueba.resultado_esperado.periodo_pasos[i]<<" "<<resultado.periodo_pasos[i]<<",";
                    
                }else{
                    std::cout<<"OK,";
                }
            }
            std::cout<<std::endl;

            std::cout<<"DIRECCION: ";
            for(int i=0;i<NUM_EJES;i++){
                if(resultado.direccion[i] != prueba.resultado_esperado.direccion[i]){
                    std::cout<<"ERROR,";
                    
                }else{
                    std::cout<<"OK,";
                }
            }
            std::cout<<std::endl;

            std::cout<<"POSICIONES ACTUALES: ";
            for(int i=0;i<NUM_EJES;i++){
                std::cout<<" "<<posicion[i]<<", ";
            }
            std::cout<<std::endl;
            std::cout<<std::endl;
        }else{
            std::cout<<obtener_error(error, " TEST")<<std::endl;
        }
        // Comparar resultado con prueba.resultado_esperado y error con prueba.error_esperado
    }

    int i = 3;
    
    for ( PruebaTrayectoria& prueba : pruebas_error) {
        int error = OK;
        calculadora = new CalculadoraTrayectorias(reader);
        
        parametros_actuadores resultado = calculadora->calcular_trayectoria_lineal(prueba.instruccion, posicion, prueba.unidades, true, &error);
        if(error == prueba.error_esperado){     
            std::cout<<"TEST "<<i<<": OK";
        }else{
            std::cout<<"TEST "<<i<<": ERROR";
        }
        i++;
        std::cout<<std::endl;
        // Verificar que el error sea el esperado
    }
    return 0;
}