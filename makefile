CC = g++
CFLAGS = -Wall -Wextra

SRCS = main.cpp maquina.cpp herramienta.cpp errores.cpp interprete.cpp trayectorias.cpp actuadores.cpp inih/ini.c inih/cpp/INIReader.cpp
OBJS = $(patsubst %.cpp, build/%.o, $(patsubst %.c, build/%.o, $(SRCS)))

main: $(OBJS) build/
	$(CC) $(CFLAGS) $(OBJS) -lwiringPi -o $@

build/%.o: %.cpp build/
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Regla para compilar los archivos .c
build/%.o: %.c build/
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/:
	mkdir -p build/inih/cpp
.PHONY: clean
clean:
	rm -rf build main.exe

