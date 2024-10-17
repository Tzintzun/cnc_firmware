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

parametros_actuadores CalculadoraTrayectorias::calcular_trayectoria_lineal(Instruccion instruccion, double *posicion_actual, bool unidades, bool sistema_cordenadas, int *error){
    parametros_actuadores parametros;
    gcode_valores valores = instruccion.valores;
    double *aux = (double *)&valores;
    double vector = 0.0;
    double componentes[NUM_EJES];
    memset(componentes,0, sizeof(double)*3);

    /*Calculamos los pasos * milimetro*/
    for(int j=0, i= X_PALABRA; j<NUM_EJES; j++){
        
        
        
        if((valores.bandera_palabras & i) != 0){
            
            double distancia = 0.0;
            if(sistema_cordenadas){
                distancia = aux[j] - posicion_actual[j];
                if(distancia == -0 || distancia == 0){
                    distancia = 0;
                }
                if(aux[j] >= area_trabajo[j]){
                    FAIL_CALCULO_TRAYECTORIA(ERROR_TRAYECTORIA_FUERA_AREA);
                }
            }else{
                distancia = aux[j];
                if(aux[j]+posicion_actual[j] >= area_trabajo[j]){
                    FAIL_CALCULO_TRAYECTORIA(ERROR_TRAYECTORIA_FUERA_AREA);
                }
            }
            
            //std::cout<<distancia<<std::endl;
            
            componentes[j] = distancia;
            vector += (distancia*distancia);
            if(!unidades){
                parametros.num_pasos[j] = std::fabs(distancia)*(this->pasos_mm[j]);
            }else{
                parametros.num_pasos[j] = std::fabs(distancia)*MM_TO_INCH*(this->pasos_mm[j]);
                std::cout<<std::fabs(distancia)*(this->pasos_mm[j])<<std::endl;
            }
            if( distancia > 0){
                parametros.direccion[j] = true;
            }else if(distancia < 0){
                parametros.direccion[j] = false;
            }else if( distancia == 0){
                parametros.num_pasos[j] = 0;
            }
        }else{
            parametros.num_pasos[j] = 0;
        }
        i= i<<1;
    }

    vector = sqrt(vector);
    double tiempo = 0.0;
    switch (instruccion.getInstruccion())
    {
    case DESPLAZAMIENTO_LINEAL_LIBRE:
        std::cout<<"DESPLAZAMIENTO LINEAL LIBRE";
        tiempo = vector/(feedrate_desplazamiento); // (min/mm)*mm = min
                                                        // seg = 60*min;
                                                        // nanoseg = 1000000000*seg
        std::cout<<std::endl<<tiempo<<"\t"<<vector<<"\t"<<feedrate_desplazamiento<<std::endl;
        for(int j=0; j<NUM_EJES; j++){
            if(parametros.num_pasos[j] != 0){
                parametros.periodo_pasos[j] = (tiempo*60*1000000000)/parametros.num_pasos[j];
                std::cout<<"Periodo del paso Calculadora"<<parametros.periodo_pasos[j];
            }else{
                parametros.periodo_pasos[j] = 0;
            }
            
        }
        break;
    case INTERPOLACION_LINEAL:

        if(instruccion.valores.bandera_palabras & F_PALABRA && (instruccion.valores.f > 0.0)){ //configurar una velocidad minima;
            if(!unidades){
                tiempo = vector/(instruccion.valores.f);
            }else{
                tiempo = vector/(instruccion.valores.f*MM_TO_INCH);
            }
            
            
            for(int j=0; j<NUM_EJES; j++){
                if(parametros.num_pasos[j] != 0){
                    parametros.periodo_pasos[j] = (tiempo*60*1000000000)/parametros.num_pasos[j];
                }else{
                    parametros.periodo_pasos[j] = 0;
                }
                
            }
        }else{
            double tiempo = vector/(feedrate_desplazamiento);
            std::cout<<tiempo<<std::endl;
            for(int j=0; j<NUM_EJES; j++){
                if(parametros.num_pasos[j] != 0){
                    parametros.periodo_pasos[j] = (tiempo*60*1000000000)/parametros.num_pasos[j];
                }else{
                    parametros.periodo_pasos[j] = 0;
                }
            }
        }
        break;
    
    default:
        FAIL_CALCULO_TRAYECTORIA(ERROR_INSTRUCCION_SIN_DESPLAZAMIENTO);
        break;
    }
    *error = OK;
    for(int i=0; i<NUM_EJES;i++ ){
        
        if(sistema_cordenadas){
            if(!unidades){
                posicion_actual[i] = aux[i];
            }else{
                posicion_actual[i] = aux[i] * MM_TO_INCH;
            }
            
        }else{
            if(!unidades){
                posicion_actual[i] += aux[i];
            }else{
                posicion_actual[i] += (aux[i] * MM_TO_INCH);
            }
           
        }
            
        
    }
    return parametros;
}