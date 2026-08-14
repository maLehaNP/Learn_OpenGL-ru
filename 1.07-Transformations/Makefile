#SHELL = /bin/sh  # Типа обязательно должно быть

# Компилятор
CC = g++
# Библиотеки для OpenGL
GLLIBS = -lGL -lglfw -lGLEW
# Флаги компилятора
CFLAGS = -g -I ../include
# Флаги линкера
LDFLAGS = -lstdc++ -lm $(GLLIBS)
# Сурсы
SOURCES = src/main.cpp

#EXECUTABLE=main


all: main

main: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm main

run:
	./main

