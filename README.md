# Manual de usuario: Firmware para la manipulación de máquinas CNC 
Firmware para el sistema embebido de manipulación de máquinas CNC. TT 2024-B038 ESOCM IPN

## Requisitos del sistema
- Raspberry Pi Zero 2 W
- Sistema operativo Raspberry Pi OS

## Instalación
### Prerrequisitos 
- Biblioteca WiringPi. (https://github.com/WiringPi/WiringPi?tab=readme-ov-file#installing)
- Biblioteca readline GNU
- Git
```shell
sudo apt-get install git libreadline-dev
```

Descargar git y descargar el repositorio.
```shell
sudo apt install git
git clone 
```

El proyecto utiliza la biblioteca INIH, por lo que tenemos que descargar la biblioteca dentro del proyecto.
```shell
cd cnc_firmware
git clone https://github.com/benhoyt/inih.git
```

Se compila el proyecto con MAKE. Si se requiere hacer alguna modificación, consulte la Documentación Técnica.
```shell
make
```
<div style="page-break-after: always;"></div>

Se genera un archivo ejecutable con el nombre `main`.
```shell
# Ejecutamos el programa
$ ./main
```

## Configuración

El sistema se puede configurar a través de un archivo de nombre "cnc.ini" que se debe encontrar en el directorio de ejecución del programa. Este archivo contiene las siguientes configuraciones:

```ini
[MaquinaCNC]
MAX_X_DIM=300.0
MAX_Y_DIM=300.0
MAX_Z_DIM=45.0

[Trayectorias]
PASOS_MM_X=5
PASOS_MM_Y=5
PASOS_MM_Z=25

FEEDRATE=500.0


[PINOUT_ACTUADORES]

PIN_EJE_X=0
PIN_EJE_Y=2
PIN_EJE_Z=3

PIN_DIR_EJE_X=4
PIN_DIR_EJE_Y=5
PIN_DIR_EJE_Z=6

PIN_HABILITAR_EJES=7


[PINOUT_HERRAMIENTA]
PIN_HABILITAR_HERRAMIENTA=25
PIN_DIR_HERRAMIENTA=27
```
<div style="page-break-after: always;"></div>

<table>
    <thead>
        <tr>
            <th>Parámetro</th>
            <th align="center">Descripción</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td align="center">MAX_X_DIM</td>
            <td rowspan=3>Dimensión de los ejes de la máquina.</td>
        </tr>
        <tr>
            <td align="center">MAX_Y_DIM</td>
        </tr>
        <tr>
            <td align="center">MAX_Z_DIM</td>
        </tr>
        <tr>
            <td>PASOS_MM_X</td>
            <td rowspan=3>Pasos por milímetro para cada eje de movimiento. Este valor depende de la configuración mecánica de la máquina.</td>
        </tr>
        <tr>
            <td>PASOS_MM_Y</td>
        </tr>
        <tr>
            <td>PASOS_MM_Z</td>
        </tr>
        <tr>
            <td>FEEDRATE</td>
            <td>Velocidad de avance por defecto. Se utiliza en el comando G0 y en caso de que el argumento F no esté presente en el comando G1. Las unidades son mm/minuto o pulgadas/minuto dependiendo del sistema de unidades.</td>
        </tr>
        <tr>
            <td>PIN_EJE_X</td>
            <td rowspan=3>Pin GPIO para la señal PWM que controla el número de pasos del motor.</td>
        </tr>
        <tr>
            <td>PIN_EJE_Y</td>
        </tr>
        <tr>
            <td>PIN_EJE_Z</td>
        </tr>
        <tr>
            <td>PIN_DIR_EJE_X</td>
            <td rowspan=3>Pin GPIO para la señal que controla la dirección de giro de los motores a pasos.</td>
        </tr>
        <tr>
            <td>PIN_DIR_EJE_Y</td>
        </tr>
        <tr>
            <td>PIN_DIR_EJE_Z</td>
        </tr>
        <tr>
            <td>PIN_HABILITAR_EJES</td>
            <td>Pin GPIO que habilita los motores de cada uno de los ejes.</td>
        </tr>
        <tr>
            <td>PIN_HABILITAR_HERRAMIENTA</td>
            <td>Pin que activa la herramienta. Ya que cada máquina CNC puede tener una distinta herramienta, este pin solo tiene los estados de Alto y Bajo. Si se requiere una funcionalidad específica, consulte la Documentación Técnica.</td>
        </tr>
        <tr>
            <td>PIN_DIR_HERRAMIENTA</td>
            <td>Pin que define la dirección de la herramienta. Ya que cada máquina CNC puede tener una distinta herramienta, este pin solo tiene los estados de Alto y Bajo. Si se requiere una funcionalidad específica, consulte la Documentación Técnica.</td>
        </tr>
    </tbody>
</table>

La disposición de los pines es la establecida por la biblioteca WiringPi, la cual se puede consultar en [pinout.xyz](https://pinout.xyz/pinout/wiringpi).

## Uso
Al ejecutar la herramienta se despliega la información de configuración de la máquina y se habilita un prompt en el que se pueden ejecutar bloques de código G. La sintaxis básica se puede ver en la siguiente imagen.
![Sintaxis código G](./sintaxis_bloque_g.png)

La lista de códigos G y M se presenta a continuación.

### Comandos G Aceptados

| Comando | Argumentos | Descripción |
|---------|------------|-------------|
| G0      | X… Y… Z…   | Desplazamiento lineal rápido. Donde X, Y, Z corresponden a los ejes de movimiento. Los argumentos X, Y, Z son opcionales, pero se debe usar al menos uno. |
| G1      | X… Y… Z… F…| Interpolación lineal. Donde X, Y, Z corresponden a los ejes de movimiento. Estos argumentos son opcionales, pero se tiene que usar uno al menos. El argumento F corresponde a la velocidad de avance de la herramienta. |
| G20     | Sin Argumentos | Configura el sistema de unidades en pulgadas. |
| G21     | Sin Argumentos | Configura el sistema de unidades en milímetros. |
| G90     | Sin argumentos | Incremento absoluto. Los valores de los ejes (X, Y, Z), en los argumentos de los comandos de desplazamiento, representan posiciones en el sistema de coordenadas. |
| G91     | Sin argumentos | Incremento relativo. Los valores de los ejes (X, Y, Z), en los argumentos de los comandos de desplazamiento, representan incrementos desde la posición actual de la máquina. |
| G92     | X… Y… Z… | Configura el punto actual como origen de la herramienta. |

## Comandos M Aceptados
| Comando | Descripción |
|---------|-------------|
| M0      | Parar el programa temporalmente. Al presionar el botón de reanudación, la máquina continúa en la siguiente línea. |
| M2      | Detener el programa. Ninguna instrucción después de M2 es ejecutada. Cuando se reinicia el ciclo de maquinado, el programa regresa al inicio del archivo. |
| M3      | Inicia el giro de la herramienta en sentido horario. |
| M4      | Inicia el giro de la herramienta en sentido antihorario. |
| M5      | Detiene la herramienta. |

## Archivos de programa Gcode

El sistema cuenta con la capacidad de leer archivos con programas Gcode. Estos programas deben pertenecer al conjunto de comandos anteriormente presentados y pueden contener saltos de línea. 

### Ejemplo
```gcode
G21
G90 

G0 Z5
G0 X100 Y100

G0 Z-0.5

G1 Y200 F500
G1 X130 F500
G1 Z-0.8 F500
G1 X200
G1 Y150 F500
G1 Z-0.5 F500
G1 Y100 F500
G1 X100 F500

G1 X150 Y150 F500
G1 X200 Y100 F500

G0 Z5
G0 X100 Y200 F500
G0 Z-0.5

G1 X150 Y150 F500
G1 X167 Y167 F500
G1 Z-0.8 F500
G1 X200 Y200 F500

M2
```
Desafortunadamente, el sistema no soporta comentarios. 

Para ejecutar un archivo de programa Gcode se tiene que pasar la ruta del archivo como parámetro al ejecutar el sistema. 

```shell
$ ./main /ruta/a/tu/archivo.gcode
```
