#include "trayectorias.h"

CalculadoraTrayectorias::CalculadoraTrayectorias(INIReader reader_conf){
    this->area_trabajo[0] = reader_conf.GetReal("MaquinaCNC", "MAX_X_DIM", MAX_X_DIS);
    this->area_trabajo[1] = reader_conf.GetReal("MaquinaCNC", "MAX_Y_DIM", MAX_Y_DIS);
    this->area_trabajo[2] = reader_conf.GetReal("MaquinaCNC", "MAX_Z_DIM", MAX_Z_DIS);

    this->pasos_mm[0] = reader_conf.GetReal("Trayectorias", "PASOS_MM_X", PASOS_X_MM);
    this->pasos_mm[1] = reader_conf.GetReal("Trayectorias", "PASOS_MM_Y", PASOS_Y_MM);
    this->pasos_mm[2] = reader_conf.GetReal("Trayectorias", "PASOS_MM_Z", PASOS_Z_MM);

    this->feedrate_desplazamiento = reader_conf.GetReal("Trayectorias", "FEEDRATE", FEEDRATE);
}

std::string CalculadoraTrayectorias::toString(){
    std::string calculadora = "\nPASOS x MM: ";

    for(int i  = 0; i<NUM_EJES; i++){
        calculadora += "\t" + std::to_string(this->pasos_mm[i]);
    }
    return calculadora;
}

parametros_actuadores CalculadoraTrayectorias::calcular_trayectoria_lineal(Instruccion instruccion, double *posicion_actual, bool unidades, int *error){
    parametros_actuadores parametros;
    gcode_valores valores = instruccion.valores;
    double *aux = (double *)&valores;
    double vector = 0.0;
    double componentes[NUM_EJES];

    /*Calculamos los pasos * milimetro*/
    for(int j=0; j<NUM_EJES; j++){
        if((valores.bandera_palabras & (X_PALABRA+j)) == 1){
            double distancia = aux[j] - posicion_actual[j];
            componentes[j] = distancia;
            vector += (distancia*distancia);
            if(unidades){
                parametros.num_pasos[j] = std::fabs(distancia)*(this->pasos_mm[j]);
            }else{
                parametros.num_pasos[j] = std::fabs(distancia)*MM_TO_INCH*(this->pasos_mm[j]);
            }
            if( distancia > 0){
                parametros.direccion[j] = true;
            }else if(distancia < 0){
                parametros.direccion[j] = false;
            }else{
                parametros.num_pasos[j] = 0;
            }
        }else{
            parametros.num_pasos[j] = 0;
        }
    }

    vector = sqrt(vector);

    switch (instruccion.getInstruccion())
    {
    case DESPLAZAMIENTO_LINEAL_LIBRE:
        double tiempo = vector/(feedrate_desplazamiento); // (min/mm)*mm = min
                                                        // seg = 60*min;
                                                        // nanoseg = 1000000000*seg
        for(int j=0; j<NUM_EJES; j++){
            parametros.periodo_pasos[j] = (tiempo*60*1000000000)/parametros.num_pasos[j];
        }
        break;
    case INTERPOLACION_LINEAL:

        if(instruccion.valores.bandera_palabras & F_PALABRA && (instruccion.valores.f > 0.0)){ //configurar una velocidad minima;
            double tiempo = vector/(instruccion.valores.f);
            for(int j=0; j<NUM_EJES; j++){
                parametros.periodo_pasos[j] = (tiempo*60*1000000000)/parametros.num_pasos[j];
            }
        }else{
            double tiempo = vector/(feedrate_desplazamiento);
            for(int j=0; j<NUM_EJES; j++){
                parametros.periodo_pasos[j] = (tiempo*60*1000000000)/parametros.num_pasos[j];
            }
        }
        


        break;
    
    default:
        FAIL_CALCULO_TRAYECTORIA(ERROR_INSTRUCCION_SIN_DESPLAZAMIENTO);
        break;
    }
}