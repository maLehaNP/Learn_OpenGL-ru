CC=g++
GLLIBS=-lglfw -lGLEW -lGL
CLIBS=-lstdc++ -lm
CFLAGS=-g -I../include $(CLIBS) $(GLLIBS)
NAME=main
SOURCES=src/main.cpp

all:
	$(CC) $(SOURCES) -o $(NAME) $(CFLAGS)

clean:
	rm $(NAME)

