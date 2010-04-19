MACHINE=$(shell uname -s)

CFLAGS = -Wall -O3
CC = g++
LDFLAGS = -lGL -lglut
ifeq ($(MACHINE), Darwin)
LDFLAGS = -framework OpenGL -framework GLUT
endif

EXE=temp
SOURCES=main.cpp
OBJECTS=$(SOURCES:.cpp=.o)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(EXE)
