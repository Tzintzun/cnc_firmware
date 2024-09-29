


/*PINES GPIO EJES*/
//SE UTILIZA EL NUMERO DE PIN DEFINIDO POR LA BIBLIOTECA WiringPi
//https://pinout.xyz/pinout/wiringpi

#include <wiringPi.h>
#define PIN_EJE_X 0
#define PIN_EJE_Y 2
#define PIN_EJE_Z 3

#define PIN_DIR_EJE_X 4
#define PIN_DIR_EJE_Y 5
#define PIN_DIR_EJE_Z 6

#define PIN_HABILITAR_EJES 7


/*PINES GPIO HERRAMIENTA*/
#define PIN_HABILITAR_HERRAMIENTA 25
#define PIN_DIR_HERRAMIENTA 27


/*FUNCIONES ACTUADORES*/
#define CONFIGURAR_PIN_SALIDA(x) (pinMode((x), OUTPUT)); (pullUpDnControl((x),PUD_DOWN))
#define DESHABILITAR_PIN(x) (digitalWrite((x), LOW))
#define HABILITAR_PIN(x) (digitalWrite((x), HIGH))
#define EJECUTAR_PASO(x)  (digitalWrite(x, HIGH));  (delay(10)); (digitalWrite(x, LOW))
#define CAMBIAR_DIRECCION_EJE(x,y) (digitalWrite((x),(y)))
#define HABILITAR_EJES(x) (digitalWrite((x), LOW))
#define DESHABILITAR_EJES(x) (digitalWrite((x), HIGH))

#define CAMBIAR_DIRECCION_HERRAMIENTA(x) (digitalWrite(PIN_DIR_HERRAMIENTA, (x)))
#define HBILITAR_HERRAMIENTA(x) (digitalWrite(PIN_HABILITAR_HERRAMIENTA, (x)))
