#include "interprete.h"


#define SIN_INSTRUCCION 0
#define INSTRUCCION_CON_ARGUMENTOS_XYZ 1
bool leer_flotante(std::string linea, int* indice, double* valor);

std::queue<Instruccion*> Interprete::interpretar_bloque_gcode(std::string linea, int* error){

    int indice_caracter = 0;
    double valor = 0;
    int bandera_palabra = 0;
    int bandera_comando = 0;
    int valor_N=0;
    std::list<Instruccion *> instrucciones_bloque;
    Instruccion* instruccion_xyz = NULL;
    Instruccion* aux = NULL;

    while(linea[indice_caracter] != '\0'){

    

        char letra = linea[indice_caracter];
        if(letra == ' '){
            indice_caracter++;
            continue;
        }
        if(letra<'A' || 'Z'<letra) {FAIL_INTERPRETE(ERROR_LETRA_ESPERADA);}
        indice_caracter++;
        if(!leer_flotante(linea, &indice_caracter, &valor)){
            FAIL_INTERPRETE(ERROR_FORMATO_NUMERO_INCORRECTO);
        }

        int valor_entero = std::round(valor);
        int tipo_instruccion = 0;

        switch (letra){
            case 'G':
                switch (valor_entero)
                {
                    case 0: case 1:
                        if((bandera_comando &= 1<<GRUPO_MODAL_1) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                            
                        }
                        if(tipo_instruccion == INSTRUCCION_CON_ARGUMENTOS_XYZ){
                            FAIL_INTERPRETE(ERROR_MULTIPLES_INSTRUC_CON_ARGUMENTOS);
                        }
                        tipo_instruccion = INSTRUCCION_CON_ARGUMENTOS_XYZ;
                        instruccion_xyz = new Instruccion();
                        switch (valor_entero)
                        {
                            case 0:
                                instruccion_xyz->setInstruccion(DESPLAZAMIENTO_LINEAL_LIBRE);
                                break;
                            case 1:
                                instruccion_xyz->setInstruccion(INTERPOLACION_LINEAL);
                                break;
                            default:
                                break;
                        }
                        bandera_comando |= 1<<GRUPO_MODAL_1;
                        break;
                    case 90: case 91:
                        if((bandera_comando &= 1<<GRUPO_MODAL_3) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                        }
                        aux = new Instruccion();
                        switch (valor_entero)
                        {
                        case 90:
                            aux->setInstruccion(MODO_DISTANCIA_ABSOLUTO);
                            break;
                        case 91:
                            aux->setInstruccion(MODO_DISTANCIA_INCREMENTAL);
                            break;
                        }
                        
                        instrucciones_bloque.push_back(aux);
                        bandera_comando |= 1<<GRUPO_MODAL_3;
                        break; 
                    case 20: case 21:
                        if((bandera_comando &= 1<<GRUPO_MODAL_6) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                        }
                        aux = new Instruccion();
                        switch (valor_entero)
                        {
                        case 20:
                            aux->setInstruccion(CONF_UNIDADES_MILIMETROS);
                            break;
                        case 21:
                            aux->setInstruccion(CONF_UNIDADES_PULGADAS);
                            break;
                        }
                        
                        instrucciones_bloque.push_back(aux);
                        bandera_comando |= 1<<GRUPO_MODAL_6;
                        break; 
                    case 28:
                        
                        if((bandera_comando &= 1<<GRUPO_MODAL_0) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                        }
                        if(tipo_instruccion == INSTRUCCION_CON_ARGUMENTOS_XYZ){
                            FAIL_INTERPRETE(ERROR_MULTIPLES_INSTRUC_CON_ARGUMENTOS);
                        }
                        tipo_instruccion = INSTRUCCION_CON_ARGUMENTOS_XYZ;
                        
                        switch (valor_entero)
                        {
                        case 28:
                            instruccion_xyz->setInstruccion(DESPLAZAMIENTO_A_CASA);
                            break;
                        }
                        
                        bandera_comando |= 1<<GRUPO_MODAL_0;
                        break; 
                    
                    default:
                        FAIL_INTERPRETE(INSTRUCCION_NO_SOPORTADA);
                        break;
                }
                break;
            case 'M':
                switch(valor_entero){
                    case 0: case 2:
                        if((bandera_comando &= 1<<GRUPO_MODAL_4) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                        }
                        aux = new Instruccion();
                        switch (valor_entero)
                        {
                        case 0:
                            aux->setInstruccion(PAUSAR_PROGRAMA);
                            break;
                        case 2:
                            aux->setInstruccion(FIN_PROGRAMA);
                            break;
                        }
                        
                        instrucciones_bloque.push_back(aux);
                        bandera_comando |= 1<<GRUPO_MODAL_4;
                        break; 
                    case 3: case 4: case 5:
                        if((bandera_comando &= 1<<GRUPO_MODAL_7) != 0 ){
                            FAIL_INTERPRETE(ERROR_GRUPO_MODAL_CONFLICTO);
                        }
                        aux = new Instruccion();
                        switch (valor_entero)
                        {
                        case 3:
                            aux->setInstruccion(ENCENDER_HERRAMIENTA_HORARIO);
                            break;
                        case 4:
                            aux->setInstruccion(ENCENDER_HERRAMIENTA_ANTIHORARIO);
                            break;
                        case 5:
                            aux->setInstruccion(DETENER_HERRAMIENTA);
                            break;
                        }
                        
                        instrucciones_bloque.push_back(aux);
                        bandera_comando |= 1<<GRUPO_MODAL_7;
                        break;
                    default:
                        FAIL_INTERPRETE(INSTRUCCION_NO_SOPORTADA);
                        break;
                }
                break;
            case 'N': 
                if((bandera_palabra & N_PALABRA)){
                    FAIL_INTERPRETE(ERROR_ARGUMENTO_REPETIDO);
                }
                valor_N = valor_entero;
                bandera_palabra |= N_PALABRA;
                break;
            case 'F':
                if((bandera_palabra & F_PALABRA)){
                    FAIL_INTERPRETE(ERROR_ARGUMENTO_REPETIDO);
                }
                if(instruccion_xyz != NULL){
                    instruccion_xyz->valores.f = valor;
                    bandera_palabra |= F_PALABRA;
                }
                break;
            case 'X':
                if((bandera_palabra & X_PALABRA)){
                    FAIL_INTERPRETE(ERROR_ARGUMENTO_REPETIDO);
                }
                if(instruccion_xyz != NULL){
                    instruccion_xyz->valores.x = valor;
                    bandera_palabra |= X_PALABRA;
                }
                break;
            case 'Y':
                if((bandera_palabra & Y_PALABRA)){
                    FAIL_INTERPRETE(ERROR_ARGUMENTO_REPETIDO);
                }
                if(instruccion_xyz != NULL){
                    instruccion_xyz->valores.y = valor;
                    bandera_palabra |= Y_PALABRA;
                }
                break;
            case 'Z':
                if((bandera_palabra & Z_PALABRA)){
                    FAIL_INTERPRETE(ERROR_ARGUMENTO_REPETIDO);
                }
                if(instruccion_xyz != NULL){
                    instruccion_xyz->valores.z = valor;
                    bandera_palabra |= Z_PALABRA;
                }
                break;
            default:
                FAIL_INTERPRETE(INSTRUCCION_NO_SOPORTADA);
                break;
        }
        
    }
    if(instruccion_xyz != NULL)
    {
        instrucciones_bloque.push_back(instruccion_xyz);
    }
    std::queue<Instruccion *> nuevas_instrucciones;
    for(std::list<Instruccion *>::iterator inst = instrucciones_bloque.begin(); 
        inst !=  instrucciones_bloque.end(); ++inst){
        (*inst)->valores.N = valor_N;
        (*inst)->valores.bandera_palabras = bandera_palabra;
        nuevas_instrucciones.push(*inst);
    }
    
    return nuevas_instrucciones;
    

    //return instruccion;
}


void Instruccion::setInstruccion(unsigned int  instruccion){
    this->instruccion = instruccion;
}

unsigned int Instruccion::getInstruccion(){
    return this->instruccion;
}

bool leer_flotante(std::string linea, int* indice, double* valor){

    
    int indice_aux = *indice;
    int numero_digitos = 0;
    bool negativo = false;
    //bool decimal = false;
    

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
    //std::cout<<"Numero Digitos: "<<numero_digitos<<std::endl;
    //std::cout<<"Indice: "<<*indice<<std::endl;
    //std::cout<<"Numero: "<<linea.substr(*indice, numero_digitos)<<"\n\n"<<std::endl;
    *valor = std::strtod (linea.substr(*indice, numero_digitos).c_str(), NULL);
    *indice = indice_aux;
    return true;
}

