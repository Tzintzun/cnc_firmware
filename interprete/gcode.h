

#ifndef GCODE
#define GCODE

#include <string>

//GRUPOS MODALES
#define GRUPO_MODAL_0 0 //G28
#define GRUPO_MODAL_1 1 //G0, G1
#define GRUPO_MODAL_3 3 //G90, G91
#define GRUPO_MODAL_6 6 //G20, G21
#define GRUPO_MODAL_4 4 //M0, M2
#define GRUPO_MODAL_7 7 //M3, M4, M5

//FUNCIONES G
#define DESPLAZAMIENTO_LINEAL_LIBRE 0 //G0
#define INTERPOLACION_LINEAL 1 //G1
#define CONF_UNIDADES_MILIMETROS //G20
#define CONF_UNIDADES_PULGADAS //G21
#define DESPLAZAMIENTO_A_CASA 28 //G28
#define MODO_DISTANCIA_ABSOLUTO 90 //G90
#define MODO_DISTANCIA_INCREMENTAL 91 //G91


//FUNCIONES M

#define PAUSAR_PROGRAMA 0 //M0
#define FIN_PROGRAMA 30 //M30
#define ENCENDER_HERRAMIENTA_HORARIO 3 //M3
#define ENCENDER_HERRAMIENTA_ANTIHORARIO 4 //M4
#define DETENER_HERRAMIENTA 5 // M5


typedef struct{
    double x; //eje x
    double y; //eje y
    double z; //eje z
    int N; // numero de linea
    double f; // feedrate
}gcode_valores;

typedef struct{
    double feedrate;
    unsigned long numero_linea;
    double posicion_x;
    double posicion_y;
    double posicion_z;
    
}estado_analizador;

typedef struct
{
    gcode_valores valores;

} gcode_bloque;








#endif