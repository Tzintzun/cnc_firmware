#include <iostream>
#include <wiringPi.h>

#define PIN_EJE_X 0
#define PIN_DIR_EJE_X 4
#define PIN_HABILITAR_EJES 7

int main(void){
    wiringPiSetup();
    pinMode(PIN_EJE_X,OUTPUT);
    pinMode(PIN_DIR_EJE_X,OUTPUT);
    pinMode(PIN_HABILITAR_EJES,OUTPUT);

    pullUpDnControl(PIN_EJE_X,PUD_DOWN);
    pullUpDnControl(PIN_DIR_EJE_X,PUD_DOWN);
    pullUpDnControl(PIN_HABILITAR_EJES,PUD_DOWN);

    digitalWrite(PIN_HABILITAR_EJES, LOW);
    digitalWrite(PIN_DIR_EJE_X, HIGH);

    for(int i=0; i<200; i++){
        digitalWrite(PIN_EJE_X, HIGH);
        digitalWrite(PIN_EJE_X, LOW);
        delayMicroseconds(800);
    }
    digitalWrite(PIN_HABILITAR_EJES, HIGH);
}