CC = g++
CFLAGS = -Wall -Wextra

SRCS = main.cpp maquina.cpp herramienta.cpp errores.cpp interprete/interprete.cpp
OBJS = $(patsubst %.cpp, build/%.o, $(SRCS))

main.exe: $(OBJS) build/
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: %.cpp build/
	$(CC) $(CFLAGS) -c $< -o $@

build/:
	mkdir build/
.PHONY: clean
clean:
	rm -f build/*.o main.exe

