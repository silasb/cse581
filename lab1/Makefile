MACHINE=$(shell uname -s)

CFLAGS = -Wall -O3
CC = g++
LDFLAGS = -lGL -lglut
ifeq ($(MACHINE), Darwin)
LDFLAGS = -framework OpenGL -framework GLUT
endif

EXE=lab1
SOURCES=main.cpp ppm.cpp
OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXE) *.ppm
