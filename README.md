# Firmware para manipulacion de maquina CNC 
Firmware para el sistema embebido de manipulacion CNC. TT 2024-B038 ESOCM IPN
Esta escrito en C++ y esta hecho para el sistema RaspberryPi Zero 2 W. 

## Instalacion
## Prerequisitos
- Instalar WiringPi.(https://github.com/WiringPi/WiringPi?tab=readme-ov-file#installing)
- Instalar la biblioteca readline de GNU 
```bash
sudo apt-get install libreadline-dev
```

Descargar git y descargar el repositorio.
```bash
sudo apt install git
git clone 

cd cnc_firmware
git clone https://github.com/benhoyt/inih.git
```


Compilamos el proyecto con make.
```bash
make
```

Se genera un archivo ejecutable main
