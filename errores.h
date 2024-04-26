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



std::string obtener_error(int error, std::string linea);

#endif