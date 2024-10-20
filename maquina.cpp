
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
                return OK;
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
        return ERROR_ARCHIVO_NO_HABIERTO;
    }

    error = OK;
    std::queue<Instruccion *> * cola_espera = new std::queue<Instruccion *>();
    
    std::chrono::time_point<std::chrono::high_resolution_clock> t_inicio = std::chrono::high_resolution_clock::now();
    std::future<int> *resultado_futuro = NULL;
    std::queue<std::string> lineas_archivo;
    while(!archivo_programa.eof()){

        std::getline(archivo_programa, linea);
        lineas_archivo.push(linea);
    }
    archivo_programa.close();
    while(!lineas_archivo.empty()){
        linea = lineas_archivo.front();
        /*leemos las n primeras lineas. Las interpretamos y las almacenamos*/
        if(linea.empty()) continue;
        std::queue<Instruccion *> cola_auxiliar = this->interprete_gcode->interpretar_bloque_gcode(linea,&error);

        if(error != OK) {
            obtener_error(error, linea);
            return error;
        }
        std::cout<<"Instruccion interpretada: \t"<<linea<<std::endl;
        if(cola_auxiliar.size() == 0) continue;

        /*Guardamos las instrucciones en la cola de espera*/

        while(!cola_auxiliar.empty()){
            cola_espera->push(cola_auxiliar.front());
            cola_auxiliar.pop();
        }

        if((cola_espera->size() >= BUFFER_INSTRUCCIONES_MAX) || (lineas_archivo.size() == 1)){
            this->cola_ejecucion = new std::queue<Instruccion *>(*cola_espera);
            if(resultado_futuro != NULL){
                int resultado_cola_ejecutada = resultado_futuro->get();
                if(resultado_cola_ejecutada != OK) return resultado_cola_ejecutada;
            }
            std::packaged_task<int()> ejecutar_cola([this](){
                
                while(true){
                    if(this->cola_ejecucion->empty()){
                        return OK;
                    }
                    Instruccion *instruccion = this->cola_ejecucion->front();

                    int resultado = this->ejecutar_instruccion(instruccion);
                    if(resultado != OK) return resultado;
                    
                    this->cola_ejecucion->pop();
                    
                }

            });

            resultado_futuro = new std::future<int>();
            *resultado_futuro = ejecutar_cola.get_future();

            std::thread hilo(std::move(ejecutar_cola));
            cola_espera = new std::queue<Instruccion *>();
        }

        lineas_archivo.pop();
    }
    
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