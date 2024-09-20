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
    case ERROR_GRUPO_MODAL_CONFLICTO:
        return "Error: El bloque contiene dos o mas instruccions pertenecientes al mismo grupo modal en >>" + linea;
        break;
    case ERROR_ARGUMENTO_REPETIDO:
        return "Error: El bloque contiene uno o mas arguentos repetidos en >> " + linea;
        break;
    case INSTRUCCION_NO_SOPORTADA:
        return  "El insterprete no porta alguna instruccion en el bloque >> " + linea;
        break;
    case ERROR_INSTRUCCION_SIN_DESPLAZAMIENTO:
        return "Error: No se pudo calcular la trayectoria, ya que la instruccion no es desplazamiento";
    case ERROR_TRAYECTORIA_FUERA_AREA:
        return "Error: Los argumentos superan los limites del area de trabajo en el bloque >> " + linea;
    default:
        break;
    }
    return "ERROR" + std::to_string(error);
}