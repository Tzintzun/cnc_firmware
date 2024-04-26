CC = g++
CFLAGS = -Wall -Wextra

SRCS = main.cpp maquina.cpp herramienta.cpp errores.cpp interprete/interprete.cpp
OBJS = $(patsubst %.cpp, build/%.o, $(SRCS))

main.exe: $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f build/*.o main.exe

