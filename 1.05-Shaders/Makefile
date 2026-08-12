CC=g++
CFLAGS=-g -lglfw -lGLEW -lGL -lstdc++ -lm -Iinclude
NAME=main
SOURCES=src/main.cpp src/shader.cpp

all:
	$(CC) $(SOURCES) -o $(NAME) $(CFLAGS)

clean:
	rm $(NAME)

