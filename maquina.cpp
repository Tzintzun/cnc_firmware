
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
    this->en_ejecucion = false;

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
    std::queue<Instruccion *> cola_auxiliar;
    /*Leer archivo*/
    std::ifstream archivo_programa;
    archivo_programa.open(ruta, std::ios::in);
    std::string linea;
    if(archivo_programa.fail()){
        return ERROR_ARCHIVO_NO_HABIERTO;
    }

    error = OK;
    std::chrono::time_point<std::chrono::high_resolution_clock> t_inicio = std::chrono::high_resolution_clock::now();
    while(!archivo_programa.eof()){
        std::getline(archivo_programa,linea);

        /*leemos las n primeras lineas. Las interpretamos y las almacenamos*/
        if(linea.empty()) continue;
        cola_auxiliar = this->interprete_gcode->interpretar_bloque_gcode(linea,&error);

        if(error != OK) {
            obtener_error(error, linea);
            return error;
        }
        std::cout<<"Instruccion interpretada: \t"<<linea<<std::endl;
        if(cola_auxiliar.size() == 0) continue;

        /*concatenamos la lista de colas con la cola principal, bloqueamos el hilo de ejecucion de instucciones*/
        bloqueo_cola_instrucciones.lock();
        while(!cola_auxiliar.empty()){
            this->cola_instrucciones.push(cola_auxiliar.front());
            cola_auxiliar.pop();
        }
        int instrucciones_en_cola = this->cola_instrucciones.size();
        bloqueo_cola_instrucciones.unlock();

        if( instrucciones_en_cola>= BUFFER_INSTRUCCIONES_MAX){
            /*Verificamos el estado del hilo de ejecucion de instrucciones*/
            bloqueo_bandera_ejecucion.lock();
                if(!en_ejecucion){
                    this->hilo_ejecicion_cola_instrucciones = new std::thread(ejecutar_cola_instrucciones, this);
                }
            bloqueo_bandera_ejecucion.unlock();

            while (true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                /*Bloquemos el recurso*/
                std::unique_lock<std::mutex> lock(bloqueo_cola_instrucciones);
                if(this->cola_instrucciones.size() <= BUFFER_INSTRUCCIONES_MIN){
                    break;
                }
                std::unique_lock<std::mutex> unlock(bloqueo_cola_instrucciones);
            }
        }
    }
    if(this->hilo_ejecicion_cola_instrucciones != NULL){
        if(this->hilo_ejecicion_cola_instrucciones->joinable()){
            this->hilo_ejecicion_cola_instrucciones->join();
        }
    }
    std::chrono::time_point<std::chrono::high_resolution_clock> t_final = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracion_ejecucion = t_final - t_final;

    std::cout<<"Ejecucion de archivo: "<<ruta<< ". TERMINADA\tTiempo: "<<duracion_ejecucion.count()<<std::endl;
    return OK;

}

void MaquinaCNC::ejecutar_cola_instrucciones(){

    Instruccion *instruccion;
    bloqueo_bandera_ejecucion.lock();
    en_ejecucion = true;
    bloqueo_bandera_ejecucion.unlock();
    while(true){
        bloqueo_cola_instrucciones.lock();
        if(cola_instrucciones.empty()) {
            bloqueo_cola_instrucciones.unlock();
            break;
        };
        instruccion = cola_instrucciones.front();
        bloqueo_cola_instrucciones.unlock();

        int resultado = ejecutar_instruccion(instruccion);
        if(resultado != OK){
            
            bloqueo_bandera_ejecucion.lock();
            hilo_error = resultado;
            en_ejecucion = false;
            bloqueo_bandera_ejecucion.unlock();
            return;
        }
        bloqueo_cola_instrucciones.lock();
        cola_instrucciones.pop();
        bloqueo_cola_instrucciones.unlock();
    }

    bloqueo_bandera_ejecucion.lock();
    hilo_error = OK;
    en_ejecucion = false;
    bloqueo_bandera_ejecucion.unlock();
    return;
    
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