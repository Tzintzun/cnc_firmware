
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
    this->sistema_unidades = true;
    this->modo_desplazamiento = true;
   

}

int MaquinaCNC::ejecutar_instruccion(Instruccion *instruccion){
    std::vector<std::string> color_ejes = {
        "\033[35m", // Magenta
        "\033[34m", // Azul
        "\033[33m", // Amarillo
    };
    std::string color_instruccion = "\033[46m";
    std::string resetea_colores = "\033[0m";
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
                    if(instruccion->getInstruccion() == DESPLAZAMIENTO_LINEAL_LIBRE){
                        std::cout<<color_instruccion<<"DESPLAZAMIENTO LINEAL LIBRE"<<resetea_colores<<std::endl;
                    }else{
                        std::cout<<color_instruccion<<"INTERPOLACION LINEAL"<<resetea_colores<<std::endl;
                    }
                    parametros_actuadores parametros = this->calculadora->calcular_trayectoria_lineal(*instruccion, this->posicion_xyz, this->sistema_unidades, this->modo_desplazamiento, &error);
                    std::cout<<"Parametros:"<<std::endl;
                    for(int i=0; i<NUM_EJES; i++){
                        std::cout<<"\t"<<color_ejes[i]<<(char)(i+88)<<"(NUMERO PASOS:"<<parametros.num_pasos[i]<<", PERIODO PASO:"<<parametros.periodo_pasos[i]<<")"<<resetea_colores<<std::endl;
                    }
                    if(error != OK){
                        return error;
                    }

                    error = this->manipulador_actuadores->ejecutar_movimiento(parametros);
                    if(error != OK) return error;
                }

                break;
            case CONF_UNIDADES_MILIMETROS:
                {
                    std::cout<<color_instruccion<<"CONFIGURANDO UNIDADES EN MILIMETROS"<<resetea_colores<<std::endl;
                    this->sistema_unidades = true;
                }
                break;
            case CONF_UNIDADES_PULGADAS:
                {
                    std::cout<<color_instruccion<<"CONFIGURANDO UNIDADES EN PULGADAS"<<resetea_colores<<std::endl;
                    this->sistema_unidades = false;
                }
                break;
            case MODO_DISTANCIA_ABSOLUTO: 
                {
                    std::cout<<color_instruccion<<"CONFIGURANDO SISTEMA DE CORDENADAS ABSOLUTAS"<<resetea_colores<<std::endl;
                    this->modo_desplazamiento = true;
                }
                break;
            case MODO_DISTANCIA_INCREMENTAL:
                {
                    std::cout<<color_instruccion<<"CONFIGURANDO SISTEMA DE CORDENADAS RELATIVAS"<<resetea_colores<<std::endl;
                    this->modo_desplazamiento = false;                    
                }
                break;
            case TRASLADO_ORIGEN:
                {
                    //std::cout<<color_instruccion<<"TRADALANDO POSICION DE LA MAQUINA"<<resetea_colores<<std::endl;
                    gcode_valores valores= instruccion->valores;
                    long *valores_ejes = (long *)&valores; 
                    std::cout<<"Nueva posicion:"<<std::endl;
                    for(int i=0, j = X_PALABRA; i<NUM_EJES; i++){
                        if((valores.bandera_palabras & j) != 0){
			    //std::cout<<j<<" "<<valores_ejes[i]<<std::endl<<instruccion->toString()<<std::endl;
                            this->posicion_xyz[i] = valores_ejes[i];
                        }
                        std::cout<<color_ejes[i]<<(char)(i+88)<<": "<<this->posicion_xyz[i]<<resetea_colores<<std::endl;
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
                std::cout<<color_instruccion<<"ESPERA"<<resetea_colores<<std::endl;
                std::cout<<"Pulsa una tecla para continuar"; //posteriormente se puede programar la lectura de un boton externo.
                std::cin.get();
                break;
            case FIN_PROGRAMA:
                std::cout<<color_instruccion<<"FIN DEL PROGRAMA"<<resetea_colores<<std::endl;
                return PROGRAMA_TERMINADO;
                break;
            case ENCENDER_HERRAMIENTA_HORARIO:
                std::cout<<color_instruccion<<"HERRAIENTA HABILITADA SENTIDO HORARIO"<<resetea_colores<<std::endl;
                manipulador_actuadores->deshabilitar_herramienta();
                manipulador_actuadores->establecer_herramienta_sentido_horario();
                manipulador_actuadores->habilitar_herramienta();
                break;
            case ENCENDER_HERRAMIENTA_ANTIHORARIO:
                std::cout<<color_instruccion<<"HERRAIENTA HABILITADA SENTIDO ANTI-HORARIO"<<resetea_colores<<std::endl;
                manipulador_actuadores->deshabilitar_herramienta();
                manipulador_actuadores->establecer_herramienta_sentido_antihorario();
                manipulador_actuadores->habilitar_herramienta();
                break;
            case DETENER_HERRAMIENTA:
                std::cout<<color_instruccion<<"HERRAMIENTA DES-HABILITADA"<<resetea_colores<<std::endl;
                manipulador_actuadores->deshabilitar_herramienta();
                break;
            default:
                std::cout<<color_instruccion<<"INSTRUCCION NO SOPORTADA"<<resetea_colores<<std::endl;
                return INSTRUCCION_NO_SOPORTADA;
                break;
        }
        break;
    }
    std::cout<<"\033[32m"<<"<<COMANDO EJECUTADO EXITOSAMENTE>>"<<"\033[0m"<<std::endl;
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
    int num_lineas = 0;
    bool fin_programa = false;
    while(!archivo_programa.eof() && !fin_programa ){

        std::getline(archivo_programa, linea);
        if(linea.empty()) continue;
	std::cout<<"~"<<num_lineas<<": "<<linea<<std::endl;
        std::queue<Instruccion *> cola_auxiliar = this->interprete_gcode->interpretar_bloque_gcode(linea,&error);
        if(error != OK) {
            std::cout<<obtener_error(error, linea)<<std::endl;
            return error;
        }
        while(!cola_auxiliar.empty()){
            Instruccion *instruccion = cola_auxiliar.front();
            int resultado = this->ejecutar_instruccion(instruccion);
            if(resultado != OK && resultado != PROGRAMA_TERMINADO){
                std::cout<<"\033[31m"<<obtener_error(resultado, linea)<<"\033[0m"<<std::endl;
                return resultado;
            }
            if(resultado == PROGRAMA_TERMINADO){
                fin_programa =true;
                break;
            }
            cola_auxiliar.pop();
        }
        //std::cout<<"OK"<<std::endl;
	num_lineas++;
    }
    archivo_programa.close();
    
    
    std::chrono::time_point<std::chrono::high_resolution_clock> t_final = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duracion_ejecucion = t_final - t_inicio;

    std::cout<<"Ejecucion de archivo: "<<ruta<< ".\033[32m TERMINADA CON EXITO\033[0m\tTiempo: "<<duracion_ejecucion.count()<<"s"<<std::endl;
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
