#include <iostream>
#include <ctime>

typedef struct 
{
    long numero_pasos;
    long periodo;
    int pin;
    timer_t *timer;
    bool estado;
}configuracion_actuador;


int main(void){

    unsigned long t0, t1;
    configuracion_actuador *actuador = new configuracion_actuador;
    actuador->numero_pasos = 200;
    actuador->pin = 0;
    t0 = clock();
    std::cout<<"EJECUTANDO: PIN: "<<actuador->pin<<"\tPASOS R: "<<actuador->numero_pasos<<std::endl;
    t1 = clock();

    unsigned long tiempo = (t1 - t0);
    std::cout<<tiempo<<std::endl;

    return 0;
}