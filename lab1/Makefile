MACHINE=$(shell uname -s)

CFLAGS = -Wall -std=c99
CC = cc
LDFLAGS = -lGL -lglut
ifeq ($(MACHINE), Darwin)
LDFLAGS = -framework OpenGL -framework GLUT
endif

EXE=lab1
SOURCES=main.c ppm.c
OBJECTS = $(SOURCES:.c=.o)

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXE)

main: main.o 
