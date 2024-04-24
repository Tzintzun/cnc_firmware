#include "interprete.h"
#include "../errores.h"
#include <iostream>
bool leer_flotante(std::string linea, int* indice, double* valor);

Instruccion* Interprete::interpretar_bloque_gcode(std::string linea, int* error){

    int indice_caracter = 0;
    double valor = 0;
    while(linea[indice_caracter] != '\0'){
        char letra = linea[indice_caracter];
        if(letra<'A' || 'Z'<letra) {*error = ERROR_LETRA_ESPERADA; return NULL;}
        indice_caracter++;
        if(!leer_flotante(linea, &indice_caracter, &valor)){
            *error = ERROR_FORMATO_NUMERO_INCORRECTO;
            return NULL;
        }

        
    }
    return new Instruccion(DESPLAZAMIENTO_LINEAL_LIBRE);

}

Instruccion::Instruccion(unsigned int instruccion){
    this->instruccion = instruccion;
}

bool leer_flotante(std::string linea, int* indice, double* valor){

    
    int indice_aux = *indice;
    int numero_digitos = 0;
    bool negativo = false;
    bool decimal = false;

    

    if(linea[indice_aux] == '-'){
        negativo = true;
        indice_aux++;
        numero_digitos++;
    }else if (linea[indice_aux] == '+'){
        indice_aux++;
        numero_digitos++;
    }

    int numero = (linea[indice_aux] - '0');
    while(true){
        if(( numero>= 0 && numero<=9) || numero == ('.' - '0')){
            numero_digitos++;
            indice_aux++;
        }else{
            break;
        }
        numero = (linea[indice_aux] - '0');
    }

    if(!numero_digitos) return false;
    std::cout<<linea.substr(*indice + 1, numero_digitos)<<std::endl;
    *valor = std::strtod (linea.substr(*indice + 1, numero_digitos).c_str(), NULL);
    *indice = indice_aux;
    return true;
}

