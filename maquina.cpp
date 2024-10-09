
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

int MaquinaCNC::ejecutar_instruccion(std::string instruccion){
    int error = OK;
    std::queue<Instruccion*> instrucciones = this->interprete_gcode->interpretar_bloque_gcode(instruccion, &error);
    if(error != OK){
        return error;
    }

    while(!instrucciones.empty()){
        Instruccion *instruccion = instrucciones.front();
        switch (instruccion->tipo_instruccion){
            case TIPO_INSTRUCCION_G:
                switch (instruccion->getInstruccion())
                {
                case INTERPOLACION_LINEAL: case DESPLAZAMIENTO_LINEAL_LIBRE:

                    parametros_actuadores parametros = this->calculadora->calcular_trayectoria_lineal(*instruccion, this->posicion_xyz, this->sistema_unidades, this->modo_desplazamiento, &error);

                    if(error != OK){
                        return error;
                    }

                    error = this->manipulador_actuadores->ejecutar_movimiento(parametros);
                    if(error != OK) return error;
                    break;
                case CONF_UNIDADES_MILIMETROS:
                    this->sistema_unidades = false;
                    break;
                case CONF_UNIDADES_PULGADAS:
                    this->sistema_unidades = true;
                    break;
                case MODO_DISTANCIA_ABSOLUTO: 
                    this->modo_desplazamiento = true;
                    break;
                case MODO_DISTANCIA_INCREMENTAL:
                    this->modo_desplazamiento = false;
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
        instrucciones.pop();
    }
    return OK;
}