CC=gcc
CFLAGS=-g -lglfw -lGLEW -lGL -lstdc++ -lm -Iinclude
NAME=main
SOURCES=src/main.cpp src/shader.cpp

all:
	$(CC) -o $(NAME) $(SOURCES) $(CFLAGS)

clean:
	rm $(NAME)

