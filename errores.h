#ifndef __ERRORES__
#define __ERRORES__

#include <string>

#define OK 0
#define ERROR_LETRA_ESPERADA 1
#define ERROR_FORMATO_NUMERO_INCORRECTO 2

#define ERROR_MULTIPLES_INSTRUC_CON_ARGUMENTOS  3
#define ERROR_GRUPO_MODAL_CONFLICTO 4
#define ERROR_ARGUMENTO_REPETIDO 5

#define INSTRUCCION_NO_SOPORTADA 6
#define ERROR_BLOQUE_SIN_INSTRUCCION 7

//ERRORES EN EL CALCULO DE TRAYECTORIAS
#define ERROR_INSTRUCCION_SIN_DESPLAZAMIENTO 8
#define ERROR_TRAYECTORIA_FUERA_AREA 9


//ERRORES MANIPULACION DE ACTUADORES
#define ERROR_SENIAL_NO_CREADA 10
#define ERROR_TIMER_NO_CREADO 11
#define ERROR_TIMER_NO_CONFIGURADO 12
#define ERROR_TIMER_NO_DESTRUIDO 13

//ERRORES EJECUACION INSTRUCCIONES
#define ERROR_ARCHIVO_NO_HABIERTO 14
#define ERROR_INSTRUCCION_NULA 15

#define PROGRAMA_TERMINADO 30

std::string obtener_error(int error, std::string linea);

#endif