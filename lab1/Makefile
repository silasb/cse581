MACHINE=$(shell uname -s)

CFLAGS = -Wall
CC = g++
LDFLAGS = -lGL -lglut
ifeq ($(MACHINE), Darwin)
LDFLAGS = -framework OpenGL -framework GLUT
endif

EXE=lab1
SOURCES=main.cpp ppm.cpp
OBJECTS = $(SOURCES:.c=.o)

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXE) *.ppm

main: main.o 
