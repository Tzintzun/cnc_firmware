#include "actuadores.h"

ManipularActuadores::ManipularActuadores(INIReader reader_config){
    this->pin_eje[0] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_X", PIN_EJE_X);
    this->pin_eje[1] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_Y", PIN_EJE_Y);
    this->pin_eje[2] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_EJE_Z", PIN_EJE_Z);

    this->pin_dir_ejes[0] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_X", PIN_DIR_EJE_X);
    this->pin_dir_ejes[1] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_Y", PIN_DIR_EJE_Y);
    this->pin_dir_ejes[2] = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_DIR_EJE_Z", PIN_DIR_EJE_Z);

    this->pin_habilitar_ejes = reader_config.GetInteger("PINOUT_ACTUADORES", "PIN_HABILITAR_EJES", PIN_HABILITAR_EJES);
}


int ManipularActuadores::ejecutar_movimiento(parametros_actuadores parametros){

    struct sigaction *senial_timer;
    senial_timer = (struct sigaction *)malloc(sizeof(struct sigaction));
    senial_timer->sa_flags = SA_SIGINFO;
    senial_timer->sa_sigaction = ManipularActuadores::signal_handler;
    sigemptyset(&(senial_timer->sa_mask));

    if(sigaction(SIGRTMIN, senial_timer, nullptr) == -1){
        FAIL_MANIPULACION_ACTUADOR(ERROR_SENIAL_NO_CREADA);
    }

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
        its->it_value.tv_nsec = 0;
        its->it_interval.tv_sec = 0;
        its->it_interval.tv_nsec = parametros.periodo_pasos[i];

        if(timer_settime(timer, 0, its, nullptr) == -1){
            FAIL_MANIPULACION_ACTUADOR(ERROR_TIMER_NO_CONFIGURADO);
        }

    }
}