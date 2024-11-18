
#include "maquina.h"

MaquinaCNC::MaquinaCNC(INIReader reader){


    /*CargarConfiguracion*/
    (this->calculadora) = new CalculadoraTrayectorias(reader);
    this->dimenciones_xyz[0] = reader.GetReal("MaquinaCNC", "MAX_X_DIM", MAX_X_DIS);
    this->dimenciones_xyz[1] = reader.GetReal("MaquinaCNC", "MAX_Y_DIM", MAX_Y_DIS);
    this->dimenciones_xyz[2] = reader.GetReal("MaquinaCNC", "MAX_Z_DIM", MAX_Z_DIS);
    memset(this->posicion_xyz,0, sizeof(posicion_xyz));
    this->calculadora = new CalculadoraTrayectorias(reader);
    this->interprete_gcode = new Interprete();
    this->manipulador_actuadores = new ManipularActuadores(reader);
    this->router = new Herramienta();
    this->sistema_unidades = false;
    this->modo_desplazamiento = true;
   

}

int MaquinaCNC::ejecutar_instruccion(Instruccion *instruccion){

    if(instruccion == NULL){
        return ERROR_INSTRUCCION_NULA;
    }
    int error = OK;
    switch (instruccion->tipo_instruccion){
        case TIPO_INSTRUCCION_G:
            switch (instruccion->getInstruccion())
            {
            case INTERPOLACION_LINEAL: case DESPLAZAMIENTO_LINEAL_LIBRE:

                {
                    parametros_actuadores parametros = this->calculadora->calcular_trayectoria_lineal(*instruccion, this->posicion_xyz, this->sistema_unidades, this->modo_desplazamiento, &error);

                    if(error != OK){
                        return error;
                    }

                    error = this->manipulador_actuadores->ejecutar_movimiento(parametros);
                    if(error != OK) return error;
                }

                break;
            case CONF_UNIDADES_MILIMETROS:
                {this->sistema_unidades = false;}
                break;
            case CONF_UNIDADES_PULGADAS:
                {this->sistema_unidades = true;}
                break;
            case MODO_DISTANCIA_ABSOLUTO: 
                {this->modo_desplazamiento = true;}
                break;
            case MODO_DISTANCIA_INCREMENTAL:
                {this->modo_desplazamiento = false;}
                break;
            case TRASLADO_ORIGEN:
                {
                    gcode_valores valores= instruccion->valores;
                    long *valores_ejes = (long *)&valores; 
                    for(int i=0, j = X_PALABRA; i<NUM_EJES; i++){
                        if((valores.bandera_palabras & j) != 0){
                            this->posicion_xyz[i] = valores_ejes[i];
                        }
                        j = j<<1;
                    }
                }
                break;
            default:
                return INSTRUCCION_NO_SOPORTADA; //retornamos un error
                break;
            }
        break;
    case TIPO_INSTRUCCION_M:
        switch (instruccion->getInstruccion()){
            case PAUSAR_PROGRAMA:
                std::cout<<"Pulsa una tecla para continuar"; //posteriormente se puede programar la lectura de un boton externo.
                std::cin.get();
                break;
            case FIN_PROGRAMA:
                return PROGRAMA_TERMINADO;
                break;
            case ENCENDER_HERRAMIENTA_HORARIO:
                manipulador_actuadores->deshabilitar_herramienta();
                manipulador_actuadores->establecer_herramienta_sentido_horario();
                manipulador_actuadores->habilitar_herramienta();
                break;
            case ENCENDER_HERRAMIENTA_ANTIHORARIO:
                manipulador_actuadores->deshabilitar_herramienta();
                manipulador_actuadores->establecer_herramienta_sentido_antihorario();
                manipulador_actuadores->habilitar_herramienta();
                break;
            case DETENER_HERRAMIENTA:
                manipulador_actuadores->deshabilitar_herramienta();
                break;
            default:
                return INSTRUCCION_NO_SOPORTADA;
                break;
        }
        break;
    }
    return OK;
}



int MaquinaCNC::ejecutar_instruccion(std::string instruccion){
    int error = OK;
    std::queue<Instruccion*> instrucciones = this->interprete_gcode->interpretar_bloque_gcode(instruccion, &error);
    if(error != OK){
        return error;
    }

    while(!instrucciones.empty()){
        Instruccion *instruccion = instrucciones.front();
        int resultado = this->ejecutar_instruccion(instruccion);
        if(resultado != OK) return resultado;
        instrucciones.pop();
    }
    return OK;
}

int MaquinaCNC::ejecutar_archivo(std::string ruta){

    int error;

    /*Leer archivo*/
    std::ifstream archivo_programa;
    archivo_programa.open(ruta, std::ios::in);
    std::string linea;
    if(archivo_programa.fail()){
        std::cout<<obtener_error(ERROR_ARCHIVO_NO_HABIERTO, ruta)<<std::endl;
        return ERROR_ARCHIVO_NO_HABIERTO;
    }

    error = OK;
    std::chrono::time_point<std::chrono::high_resolution_clock> t_inicio = std::chrono::high_resolution_clock::now();
    
    
    while(!archivo_programa.eof()){

        std::getline(archivo_programa, linea);
        if(linea.empty()) continue;
        std::queue<Instruccion *> cola_auxiliar = this->interprete_gcode->interpretar_bloque_gcode(linea,&error);
        if(error != OK) {
            std::cout<<obtener_error(error, linea)<<std::endl;
            return error;
        }
        while(!cola_auxiliar.empty()){
            Instruccion *instruccion = cola_auxiliar.front();
            int resultado = this->ejecutar_instruccion(instruccion);
            if(resultado != OK){
                std::cout<<obtener_error(resultado, linea)<<std::endl;
                return resultado;
            }
            cola_auxiliar.pop();
        }
        std::cout<<"OK"<<std::endl;
    }
    archivo_programa.close();
    
    
    std::chrono::time_point<std::chrono::high_resolution_clock> t_final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_ejecucion = t_final - t_inicio;

    std::cout<<"Ejecucion de archivo: "<<ruta<< ". TERMINADA\tTiempo: "<<duracion_ejecucion.count()<<std::endl;
    return OK;

}


std::string MaquinaCNC::toString(){
    std::string maquina = "\nNUMERO EJES: " + std::to_string(NUM_EJES);
    maquina += "\nDIMENSIONES: ";
    for(int i = 0; i<NUM_EJES; i++){
        maquina += std::to_string(this->dimenciones_xyz[i]) + ",";
    }
    maquina += calculadora->toString();
    
    return maquina;
}