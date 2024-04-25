#include "interprete.h"


#define SIN_INSTRUCCION 0
#define INSTRUCCION_CON_ARGUMENTOS_XYZ 1
bool leer_flotante(std::string linea, int* indice, double* valor);

std::queue<Instruccion*> Interprete::interpretar_bloque_gcode(std::string linea, int* error){

    int indice_caracter = 0;
    double valor = 0;
    int bandera_palabra = 0;
    int bandera_comando = 0;
    std::queue<Instruccion *> instrucciones_bloque;
    Instruccion* instruccion = new Instruccion();

    while(linea[indice_caracter] != '\0'){


        char letra = linea[indice_caracter];
        if(letra<'A' || 'Z'<letra) {FAIL_INTERPRETE(ERROR_LETRA_ESPERADA);}
        indice_caracter++;
        if(!leer_flotante(linea, &indice_caracter, &valor)){
            FAIL_INTERPRETE(ERROR_FORMATO_NUMERO_INCORRECTO)
        }

        int valor_entero = std::round(valor);
        int tipo_instruccion = 0;

        switch (letra){
            case 'G':
                switch (valor_entero)
                {
                    case 0: case 1:
                        if(tipo_instruccion == INSTRUCCION_CON_ARGUMENTOS_XYZ){
                            FAIL_INTERPRETE(ERROR_MULTIPLES_INSTRUC_CON_ARGUMENTOS);
                        }
                        if((bandera_palabra &= 1<<GRUPO_MODAL_1) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                            
                        }
                        tipo_instruccion = INSTRUCCION_CON_ARGUMENTOS_XYZ;
                        switch (valor_entero)
                        {
                            case 0:
                                instruccion->setInstruccion(DESPLAZAMIENTO_LINEAL_LIBRE);
                                break;
                            case 1:
                                instruccion->setInstruccion(INTERPOLACION_LINEAL);
                                break;
                            default:
                                break;
                        }
                        bandera_palabra |= 1<<GRUPO_MODAL_1;
                        break;
                    case 90: case 91:
                        if((bandera_palabra &= 1<<GRUPO_MODAL_3) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                        }
                        Instruccion* aux = new Instruccion();
                        switch (valor_entero)
                        {
                        case 90:
                            aux->setInstruccion(MODO_DISTANCIA_ABSOLUTO);
                            break;
                        case 91:
                            aux->setInstruccion(MODO_DISTANCIA_INCREMENTAL);
                            break;
                        }
                        
                        instrucciones_bloque.push(aux);


                        break; 
                    
                    default:
                        break;
                }
                break;
            case 'M':


                break;
        }
        
    }
    //return instruccion;
}


void Instruccion::setInstruccion(unsigned int  instruccion){
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

