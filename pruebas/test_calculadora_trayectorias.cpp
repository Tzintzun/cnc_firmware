#include "../trayectorias.h"
#include "../interprete.h"
#include "../configuracion.h"
#include "../errores.h"
#include "../inih/cpp/INIReader.h"

typedef struct {
    Instruccion instruccion;
    double posicion_inicial[NUM_EJES];
    bool unidades; // false para mm, true para pulgadas
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


    Instruccion instruccion1;
    instruccion1.setInstruccion(INTERPOLACION_LINEAL); // G1
    instruccion1.valores.x = 100.0;
    instruccion1.valores.y = 50.0;
    instruccion1.valores.z = 10.0;
    instruccion1.valores.bandera_palabras = X_PALABRA | Y_PALABRA | Z_PALABRA;

    PruebaTrayectoria prueba1 = {
        instruccion1,
        {0.0, 0.0, 0.0}, // Posición inicial
        false, // Unidades en mm
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
    instruccion2.valores.x = 4.0; // pulgadas
    instruccion2.valores.y = 2.0;
    instruccion2.valores.bandera_palabras = X_PALABRA | Y_PALABRA;

    PruebaTrayectoria prueba2 = {
        instruccion2,
        {50.0, 50.0, 0.0}, // Posición inicial
        true, // Unidades en pulgadas
        { // Resultado esperado
            {257047, 268223, 0}, // num_pasos (conversión a mm)
            {7759, 7435, 0}, // periodo_pasos
            {false, false, true} // dirección
        },
        OK
    };

    pruebas_exito.push_back(prueba2);


    Instruccion instruccion3;
    instruccion3.setInstruccion(CONF_UNIDADES_PULGADAS); // G21

    PruebaTrayectoria prueba3 = {
        instruccion3,
        {0.0, 0.0, 0.0}, // Posición inicial
        false, // Unidades en mm (no importa en este caso)
        {}, // No importa el valor de retorno
        ERROR_INSTRUCCION_SIN_DESPLAZAMIENTO
    };

    pruebas_error.push_back(prueba3);

    Instruccion instruccion4;
    instruccion4.setInstruccion(INTERPOLACION_LINEAL); // G1
    instruccion4.valores.x = 400.0; // Fuera del límite de área (300 mm)
    instruccion4.valores.y = 50.0;
    instruccion4.valores.bandera_palabras = X_PALABRA | Y_PALABRA;

    PruebaTrayectoria prueba4 = {
        instruccion4,
        {0.0, 0.0, 0.0}, // Posición inicial
        false, // Unidades en mm
        {}, // No importa el valor de retorno
        ERROR_TRAYECTORIA_FUERA_AREA
    };

    pruebas_error.push_back(prueba4);

    for ( PruebaTrayectoria& prueba : pruebas_exito) {
        int error = OK;
        calculadora = new CalculadoraTrayectorias(reader);
        parametros_actuadores resultado = calculadora->calcular_trayectoria_lineal(prueba.instruccion, prueba.posicion_inicial, prueba.unidades, &error);
        
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
        }
        // Comparar resultado con prueba.resultado_esperado y error con prueba.error_esperado
    }

    for ( PruebaTrayectoria& prueba : pruebas_error) {
        int error = OK;
        calculadora = new CalculadoraTrayectorias(reader);
        
        parametros_actuadores resultado = calculadora->calcular_trayectoria_lineal(prueba.instruccion, prueba.posicion_inicial, prueba.unidades, &error);
        if(error == prueba.error_esperado){     
            std::cout<<"TEST: OK";
        }else{
            std::cout<<"TEST: ERROR";
        }
        
        std::cout<<std::endl;
        // Verificar que el error sea el esperado
    }
    return 0;
}