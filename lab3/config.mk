# version and name
VERSION = 0.01
AUTHOR = barondas

MACHINE=$(shell uname -s)

# includes and libs
INCS = -I.
LIBS = -lGL -lglut
ifeq ($(MACHINE), Darwin)
LIBS = -framework OpenGL -framework GLUT
endif

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -std=c99
CFLAGS = -g -pedantic -Wall -O0 ${INC} ${CPPFLAGS}
LDFLAGS = -g ${LIBS}

# compiler and linker
CC = gcc
