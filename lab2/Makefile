MACHINE=$(shell uname -s)

CFLAGS = -Wall -g
CC = g++
LDFLAGS = -lGL -lglut
ifeq ($(MACHINE), Darwin)
LDFLAGS = -framework OpenGL -framework GLUT
endif

EXE=lab2
SOURCES=main.cpp scene.cpp ppm.cpp
OBJECTS=$(SOURCES:.cpp=.o)

$(EXE): $(OBJECTS)
	$(LINK.cc) $^ -o $@

clean:
	rm -rf *.o $(EXE)
