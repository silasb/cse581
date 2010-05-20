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
	@cp -R frame.ppm Makefile config.mk README.textile *.h ${SRC} ${EXE}-${AUTHOR}
	@zip -r ${EXE}-${AUTHOR}.zip ${EXE}-${AUTHOR}
	@rm -rf ${EXE}-${AUTHOR}

.PHONY: clean dist
