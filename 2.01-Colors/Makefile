# Компилятор
CC = g++
# Библиотеки для OpenGL
GLLIBS = -lGL -lglfw -lGLEW
# Флаги компилятора
CFLAGS = -g -I../include -I/usr/include/imgui
# Флаги линкера
LDFLAGS = -lstdc++ -lm $(GLLIBS) -limgui -lstb
# Сурсы
SOURCES = src/main.cpp src/rendering.h


all: main

main: $(SOURCES)
	$(CC) $(SOURCES) $(CFLAGS) $(LDFLAGS) -o $@

clean:
	rm main

run:
	./main

