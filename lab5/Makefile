include config.mk

EXE = lab4
SRC = main.c events.c drawing.c shared.c ppm.c loader/obj_loader.c entity.c camera.c
OBJ = $(SRC:.c=.o)
PACKAGED_FILES = $(SRC) Makefile config.mk README.textile *.h frame.ppm Untitled.ppm

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
	@cp -R $(PACKAGED_FILES) ${EXE}-${AUTHOR}
	@zip -r ${EXE}-${AUTHOR}.zip ${EXE}-${AUTHOR}
	@rm -rf ${EXE}-${AUTHOR}

.PHONY: clean dist
