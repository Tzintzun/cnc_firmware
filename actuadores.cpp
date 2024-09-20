#include "actuadores.h"
std::map<int, configuracion_actuador *> ManipularActuadores::actuadores;
bool ManipularActuadores::temporizadores_listos = false;
ManipularActuadores::ManipularActuadores(INIReader reader_config){
    this->pin_eje[0] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_X", PIN_EJE_X);
    this->pin_eje[1] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_Y", PIN_EJE_Y);
    this->pin_eje[2] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_Z", PIN_EJE_Z);

    this->pin_dir_ejes[0] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_X", PIN_DIR_EJE_X);
    this->pin_dir_ejes[1] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_Y", PIN_DIR_EJE_Y);
    this->pin_dir_ejes[2] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_Z", PIN_DIR_EJE_Z);

    this->pin_habilitar_ejes = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_HABILITAR_EJES", PIN_HABILITAR_EJES);
    temporizadores_listos = false;

    wiringPiSetup () ;
    std::cout<<"Configurando modo pines"<<std::endl;
    for(int i = 0; i<NUM_EJES;i++){
        std::cout<<"\t"<<pin_eje[i]<<" "<<pin_dir_ejes[i]<<std::endl;
        pinMode(pin_eje[i], OUTPUT);
        pinMode(pin_dir_ejes[i], OUTPUT);
    }

    pinMode(pin_habilitar_ejes, OUTPUT);
}


int ManipularActuadores::ejecutar_movimiento(parametros_actuadores parametros){

    /*Dehabilitamos señales*/
    HABILITAR_EJES(pin_habilitar_ejes, LOW);
    temporizadores_listos = false;
    /*Configuramos la señal*/

    std::cout<<"Configurando señal"<<std::endl;
    struct sigaction *senial_timer;
    senial_timer = (struct sigaction *)malloc(sizeof(struct sigaction));
    senial_timer->sa_flags = SA_SIGINFO;
    senial_timer->sa_sigaction = ManipularActuadores::signal_handler;
    sigemptyset(&(senial_timer->sa_mask));

    if(sigaction(SIGRTMIN, senial_timer, nullptr) == -1){
        FAIL_MANIPULACION_ACTUADOR(ERROR_SENIAL_NO_CREADA);
    }

    /*Configuramos las señales de control*/

    std::cout<<"Configurando direccion de pines"<<std::endl;

    for (int i=0;i<NUM_EJES;i++){
        std::cout<<"\t"<<parametros.direccion[i]<<std::endl;
        if(parametros.direccion[i]){
            CAMBIAR_DIRECCION_EJE(pin_eje[i],HIGH);
        }else{
            CAMBIAR_DIRECCION_EJE(pin_eje[i],LOW);
        }
    }
    
    /*Configuramos los temporizadores*/
    std::cout<<"Configurando temporizadores"<<std::endl;

    for(int i=0; i<NUM_EJES;i++){
        

        timer_t *timer;
        timer = new timer_t;
        struct sigevent *sig_ev;
        sig_ev = (struct sigevent *)malloc(sizeof(struct sigevent));
        int timer_id = i;

        sig_ev->sigev_notify = SIGEV_SIGNAL;
        sig_ev->sigev_signo = SIGRTMIN;
        sig_ev->sigev_value.sival_ptr = &timer_id;

        if(timer_create(CLOCK_REALTIME, sig_ev, timer) == -1){
            FAIL_MANIPULACION_ACTUADOR(ERROR_TIMER_NO_CREADO);
        }

        struct itimerspec *its;
        its = (struct itimerspec *)malloc(sizeof(struct itimerspec));
        its->it_value.tv_sec = 0;
        its->it_value.tv_nsec = 10000000;
        its->it_interval.tv_sec = 0;
        its->it_interval.tv_nsec = parametros.periodo_pasos[i];

        configuracion_actuador *configuracion = new configuracion_actuador;
        configuracion->numero_pasos = parametros.num_pasos[i];
        configuracion->periodo = parametros.periodo_pasos[i];
        configuracion->timer = timer;
        actuadores[timer_id] = configuracion;

        
        if(timer_settime(*timer, 0, its, nullptr) == -1){
            FAIL_MANIPULACION_ACTUADOR(ERROR_TIMER_NO_CONFIGURADO);
        }

    }
    HABILITAR_EJES(pin_habilitar_ejes, HIGH);
    temporizadores_listos = true;

    
    /*Esperamos a que los pasos terminen*/

    std::cout<<"ESPERANDO..."<<std::endl;

    while (true)
    {
        int pasos_totales_restantes = 0;
        for(int i=0;i<NUM_EJES;i++){
            pasos_totales_restantes += actuadores[i]->numero_pasos;
        }
        if(pasos_totales_restantes == 0){
            break;
        }
        std::this_thread::sleep_for(std::chrono::nanoseconds(500000000));
    }

    std::map<int, configuracion_actuador *>::iterator it;
    for (it = actuadores.begin(); it != actuadores.end(); ++it){
        if(timer_delete(*(actuadores[it->first]->timer)) == -1){
            FAIL_MANIPULACION_ACTUADOR(ERROR_TIMER_NO_DESTRUIDO);
        }
    }

    return OK;
}

void ManipularActuadores::signal_handler(int signum, siginfo_t *info, void *context){

    std::cout<<"Entrando al signal_handler"<<std::endl;
    int timer_id = -1;
    if(info->si_value.sival_ptr){
        timer_id = *(reinterpret_cast<int*>(info->si_value.sival_ptr));
    }
    if(timer_id == -1){
        std::cout<<"Timer no identidicado";
    }
    if(temporizadores_listos){
        std::cout<<"EJECUTANDO"<<std::endl;

        configuracion_actuador *actuador = actuadores[timer_id];
        if(actuador->numero_pasos>0){
            EJECUTAR_PASO(actuador->pin);
            actuador->numero_pasos -= 1;
        }
    }
    

}

