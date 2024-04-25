#include "errores.h"


std::string obtener_error(int error, std::string linea){
    switch (error)
    {
    
    case ERROR_LETRA_ESPERADA:
        return "Error: Formato de palabra incorrecto en >>" + linea + "\t\n Se esperaba una letra MAYUSCUALA A-Z";
        break;
    case ERROR_FORMATO_NUMERO_INCORRECTO:
        return "Error: Formato de numero incorrecto en >>" + linea ;
        break;
    case ERROR_MULTIPLES_INSTRUC_CON_ARGUMENTOS:
        return "Error: El bloque contiene multiples instrucciones con argumentos en >>" + linea ;
        break;
    default:
        break;
    }
    return "ERROR";
}