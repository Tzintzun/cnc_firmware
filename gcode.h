

#ifndef GCODE
#define GCODE



//GRUPOS MODALES
#define GRUPO_MODAL_0 0 //G28
#define GRUPO_MODAL_1 1 //G0, G1
#define GRUPO_MODAL_3 3 //G90, G91
#define GRUPO_MODAL_6 6 //G20, G21
#define GRUPO_MODAL_4 4 //M0, M2
#define GRUPO_MODAL_7 7 //M3, M4, M5


#define INSTRUCCION_NULA -1

#define TIPO_INSTRUCCION_G true
#define TIPO_INSTRUCCION_M false

//FUNCIONES G
#define DESPLAZAMIENTO_LINEAL_LIBRE 0 //G0
#define INTERPOLACION_LINEAL 1 //G1
#define CONF_UNIDADES_MILIMETROS 20 //G20
#define CONF_UNIDADES_PULGADAS 21 //G21
#define DESPLAZAMIENTO_A_CASA 28 //G28
#define MODO_DISTANCIA_ABSOLUTO 90 //G90
#define MODO_DISTANCIA_INCREMENTAL 91 //G91


//FUNCIONES M
#define PAUSAR_PROGRAMA 0 //M0
#define FIN_PROGRAMA 2 //M2
#define ENCENDER_HERRAMIENTA_HORARIO 3 //M3
#define ENCENDER_HERRAMIENTA_ANTIHORARIO 4 //M4
#define DETENER_HERRAMIENTA 5 // M5


//ARGUMENTOS

#define N_PALABRA 1<<0
#define F_PALABRA 1<<1

/*Los ejes deben de estar en roden consecutivo
    Si se agrega un eje extra el corrimiento debe de ser 1<<5
*/
#define X_PALABRA 1<<2
#define Y_PALABRA 1<<3
#define Z_PALABRA 1<<4


//#define FEEDRATE_ACTIVO

typedef struct{
    double feedrate;
    unsigned long numero_linea;
    double posicion_x;
    double posicion_y;
    double posicion_z;
}estado_analizador;

typedef struct
{
   

} gcode_bloque;








#endif