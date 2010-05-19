include config.mk

EXE = lab3
SRC = main.c events.c drawing.c shared.c ppm.c
OBJ = $(SRC:.c=.o)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

.o:
	@echo CC $<
	@$(CC) -c $<

clean:
	rm -f ${OBJ} $(EXE)

dist: clean
	@echo creating dist tarball
	@mkdir -p ${EXE}-${AUTHOR}
	@cp -R Makefile config.mk README.textile ${SRC} ${EXE}-${AUTHOR}
	@tar -cf ${EXE}-${AUTHOR}.tar ${EXE}-${AUTHOR}
	@gzip ${EXE}-${AUTHOR}.tar
	@rm -rf ${EXE}-${AUTHOR}

.PHONY: clean dist
